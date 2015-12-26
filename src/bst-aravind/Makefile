ROOT = ../..

include $(ROOT)/common/Makefile.common

BINS = $(BINDIR)/lf-bst-aravind
PROF = $(ROOT)/src

.PHONY:	all clean

all:	main

measurements.o:
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/measurements.o $(PROF)/measurements.c

ssalloc.o:
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/ssalloc.o $(PROF)/ssalloc.c

memalloc.o: ssalloc.o
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/memalloc.o $(BUILDIR)/ssalloc.o $(PROF)/memalloc.c

bst-aravind.o: 
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/bst-aravind.o bst-aravind.c

test.o: bst-aravind.h
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/test.o $(TEST_FILE)

main: measurements.o memalloc.o ssalloc.o  bst-aravind.o test.o 
	$(CC) $(CFLAGS) $(BUILDIR)/measurements.o $(BUILDIR)/memalloc.o $(BUILDIR)/ssalloc.o $(BUILDIR)/bst-aravind.o  $(BUILDIR)/test.o -o $(BINS) $(LDFLAGS)

clean:
	-rm -f $(BINS)
