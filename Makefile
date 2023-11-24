CC=		cc
CFLAGS=		-g -Wall -O2 -Wno-unused-function
CPPFLAGS=
INCLUDES=   
PROG=		msa-tk
BINDIR=		/usr/local/bin
LIBS=		-lz
OBJS=		ident_matrix.o neighbor.o kstring.o utils.c

.SUFFIXES:.c .o
.PHONY:all clean

.c.o:
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)

msa-tk:$(OBJS) msa-tk.o
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

install:all
		install msa-tk $(BINDIR)

clean:
		rm -fr *.o msa-tk