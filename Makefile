
CC=gcc
RM=rm -f

CFLAGS=-Wall -pedantic -g
OUT=./../obj
OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

MAIN=main
all:$(MAIN)
$(MAIN):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN) -lm
$(OBJS): | obj

obj:
	@mkdir -p ../$@

$(OUT)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) $(OBJS)

