CC = gcc
CFLAGS = -Wall -Wextra -Werror -O4
LIBS = -lm

memory: memory.c
	$(CC) $(CFLAGS) -o memory memory.c $(LIBS)

plot: memory.pdf

memory.csv: memory
	sh memory.sh >memory.csv

memory.dat: memory.csv
	awk -f plotdata.awk <memory.csv >memory.dat

memory.pdf: memory.dat
	gnuplot memory.gnuplot

clean:
	-rm -f memory memory.dat memory.pdf

distclean: clean
	-rm -f memory.csv
