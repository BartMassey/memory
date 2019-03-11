# PSU CS 201 HW 5: Caches and VM
Bart Massey  
Winter 2019

In this assignment you will:

* Analyze the memory characteristics of a target machine.

* Demonstrate how memory access patterns affect machine
  performance.

* Learn a method for scripted benchmarking.

## The Assignment

1. Make sure you're working by yourself on a Linux box. If
   you are on a Linux Lab box, please use `who` and `ps` and
   move to a different box if someone is logged in and
   working. Check that the load averages are low (below 1.0)
   with `uptime`.

2. On your chosen Linux box, look at `/proc/meminfo`. This
   file contains information about your box's memory sizes.
   You can find documentation about these fields
   [here](https://www.kernel.org/doc/Documentation/filesystems/proc.txt).

   Now look at `/proc/cpuinfo`. This will tell you about
   your processor characteristics. What processor are you
   on? How many cores do you have?

   Add information about your target box to your writeup.

3. Build the `memory` application provided here (say `make
   memory`), and say `./memory --help`. You should get a
   usage message like

        memory: usage: memory (r|w|a) (1|p) <nbytes>

   The first argument is whether to do reads, writes, or
   read-after-write accesses of memory. The second argument is
   whether to do the accesses on successive words or on
   successive pages.


4. Read the code of `memory.c`. Understand what it is doing,
   and how it works.

5. Say

        time ./memory r 1 `expr 100 \* 1024 \* 1024`

   and note down the times. Then use `w` instead of `r` and
   compare. If either time is less that 0.01 seconds or greater
   than 2 seconds scale the size by a factor of 10 and try again.

   You should see reading as substantially faster than writing.

6. Make sure that `gnuplot` is available on your machine —
   install it if needed.

   Examine the `Makefile` and understand how it is used to
   make a plot.

   Briefly look at `memory.sh`, `plotdata.awk` and
   `memory.gnuplot`. You don't need to understand these in
   detail, but it would be good to get an idea of what they
   do.

   Say `make plot`. This will run for a few minutes making
   measurements. When it is done, take a look at
   `memory.pdf`, which will show the time to write memory as
   a function of size for the various modes.

7. Modify `memory.c` to add a `b` stride that strides by cache
   blocks as opposed to words or pages. Recall that a cache
   block is 8 64-bit words — 64 bytes.

   Measure the runtimes for `b` stride using the methodology
   of step 5 above.

8. (Optional, challenging) Modify the scripting to make `b`
   stride plots as well as `p` and `1` stride.

9. Say `make clean` to prepare your code for submission.
   You should still have your `memory.csv` and `memory.pdf`
   files.

## Writeup

Your submission *must* include a writeup in a text file
named `WRITEUP.txt` or `WRITEUP.md`. The writeup should
contain your name and a description of anything interesting
about your program or its development. For this assignment,
it should describe the timings you measured and what you
learned.

## Source Control

For this assignment, you are not required to use source
control. However, I strongly suggest putting all your
project files under Git control: it will avoid
backup-file messes and give you a good history of work.

## Submission

You should submit your materials (program, Makefile,
writeup) to the course website as a ZIP archive called
`memory.zip`. The ZIP archive should unzip into a directory
named `memory`: use `zip -r memory.zip memory` to make the
archive and `unzip -t memory.zip` to check it. Use the
assignment submission interface on the course website to
upload your archive.

Your submission should contain your `memory.csv` and
`memory.pdf` files. It should not contain your Git
repository.

## License

This code is available under the "MIT License". Please see
the file `LICENSE` in this distribution for license terms.
