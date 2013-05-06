#DEBUG	= -g -O0
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -std=gnu99

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lpthread -lm

SRC	=	counter.c tlc5947.c

OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	counter


counter:	counter.o tlc5947.o
	@echo $@ [Link]
	@$(CC) -o $@ counter.o tlc5947.o $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags $(BINS)
