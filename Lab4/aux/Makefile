#
# Makefile for U3 examples
#
WAVE_SRC=wave.c u3.c
WAVE_OBJ=$(WAVE_SRC:.c=.o)
WAVE_TARGET=wave

SRCS=$(wildcard *.c)
HDRS=$(wildcard *.h)

CFLAGS +=-Wall -g
LIBS=-lm -llabjackusb -lrt

all: wave

wave: $(WAVE_OBJ) $(HDRS)
	$(CC) -o $(WAVE_TARGET) $(WAVE_OBJ) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o *~ $(WAVE_TARGET)
