CC = gcc

SDIR = srcs
ODIR = objs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))

OUT = unreal.exe

make: $(OUT)

remake: clean $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) -lmpc -lmpfr -lgmp -lustr

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $<

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make remake clean
