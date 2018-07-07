sources = $(wildcard *.c)
targets = $(patsubst %.c,%,$(sources))

CFLAGS = -Wall -std=gnu99

.PHONY: all clean

all: $(targets)

clean:
	$(RM) $(targets)
