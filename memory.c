/*
 * Analyze Memory Performance
 * Bart Massey
 */

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define PAGESIZE (4 * 1024 / 8)

enum access {
  ACCESS_READ,
  ACCESS_WRITE,
  ACCESS_RAW,   /* read-after-write */
};

enum stride {
  STRIDE_1,
  STRIDE_PAGE,
};

void usage(void) {
    fprintf(stderr, "memory: usage: memory (r|w|a) (1|p) <nbytes>\n");
    exit(1);
}

/* These are asm volatile to ensure that the accesses
   actually happen. We could do this using volatile
   pointers, probably, but this way we know exactly
   what is happening. */

static inline void read_word(uint64_t *word) {
    asm volatile ("movq %0, %%rax"
                  : : "m"(*word) : "rax");
}

static inline void write_word(uint64_t *word) {
    asm volatile ("movq $1, %0"
                  : "=m"(*word) : : );
}

static inline void raw_word(uint64_t *word) {
    asm volatile ("movq $1, %0; movq %0, %%rax"
                  : "+m"(*word) : : "rax");
}

/* This will be inlined to remove the calling overhead. */
static inline void access_word(enum access a, uint64_t *word) {
    switch (a) {
    case ACCESS_READ: read_word(word); break;
    case ACCESS_WRITE: write_word(word); break;
    case ACCESS_RAW: raw_word(word); break;
    }
}

/* Actually run the memory accesses. */
void access_memory(enum access a, enum stride s,
                   size_t nwords, uint64_t *words)
{
    switch (s) {
    case STRIDE_1:
        for (size_t i = 0; i < nwords; i++)
            access_word(a, &words[i]);
        return;
    case STRIDE_PAGE:
        /*block*/ {
            size_t page_words = PAGESIZE / sizeof *words;
            for (size_t j = 0; j < page_words; j++)
                for (size_t i = 0; i + j < nwords; i += page_words)
                    access_word(a, &words[i + j]);
        }
        return;
    }
    assert(0);
}

int main(int argc, char **argv) {
    /* Parse arguments. */
    if (argc != 4)
        usage();
    if (strlen(argv[1]) != 1)
        usage();
    char access_char = argv[1][0];
    if (strlen(argv[2]) != 1)
        usage();
    char stride_char = argv[2][0];
    size_t nbytes = atol(argv[3]);
    if (nbytes <= 0)
        usage();

    /* Process access argument. */
    enum access access;
    switch(access_char) {
    case 'r': access = ACCESS_READ; break;
    case 'w': access = ACCESS_WRITE; break;
    case 'a': access = ACCESS_RAW; break;
    default: usage();
    }

    /* Process stride argument. */
    enum stride stride;
    switch(stride_char) {
    case '1': stride = STRIDE_1; break;
    case 'p': stride = STRIDE_PAGE; break;
    default: usage();
    }

    /* Get the memory we need. */
    size_t nwords = nbytes / sizeof(uint64_t);
    uint64_t *words = malloc(nwords * sizeof *words);
    if (!words) {
        perror("malloc");
        return -1;
    }

    /* Record starting wall-clock time. */
    struct timeval tv_start;
    assert(gettimeofday(&tv_start, 0) != -1);
    
    /* Run the accesses. */
    access_memory(access, stride, nwords, words);

    /* Record ending wall-clock time. */
    struct timeval tv_end;
    assert(gettimeofday(&tv_end, 0) != -1);

    /* Report the results. */
    struct timeval tv_interval;
    timersub(&tv_end, &tv_start, &tv_interval);
    double t =
        tv_interval.tv_sec + (double) tv_interval.tv_usec / 1000000.0;
    /* Avoid problems with log(). */
    if (t < 1.0e-6)
        t = 1.0e-6;
    printf("%c,%c,%ld,%f,%.6f,%.6f\n",
           access_char,
           stride_char,
           nbytes,
           log2(nbytes),
           t,
           log2(t));

    /* This gratuitous free makes valgrind happy. */
    free(words);

    return 0;
}
