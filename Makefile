CC = gcc

SDIR = srcs
ODIR = objs

SRCS1 = $(filter-out $(SDIR)/cmd.c, $(shell find $(SDIR) -name "*.c"))
OBJS1 = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS1))
OUT1 = interpreter.exe

SRCS2 = $(filter-out $(SDIR)/main.c, $(shell find $(SDIR) -name "*.c"))
OBJS2 = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS2))
OUT2 = compiler.exe

make: $(OUT1) $(OUT2)

remake: clean make

$(OUT1): $(OBJS1)
	$(CC) -o $(OUT1) $(OBJS1) -lmpc -lmpfr -lgmp -lustr

$(OUT2): $(OBJS2)
	$(CC) -o $(OUT2) $(OBJS2) -lmpc -lmpfr -lgmp -lustr

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $<

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT1)
	@rm -f $(OUT2)

.PHONY: make remake clean
