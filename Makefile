CC      ?= clang
CFLAGS  := -std=c99 -Wall -Wextra -pedantic -I/opt/homebrew/include
LDFLAGS := -L/opt/homebrew/lib -lunistring

TARGET := str
SRC    := main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean

PREFIX  ?= $(HOME)/.local
BINDIR  := $(PREFIX)/bin

install: $(TARGET)
	mkdir -p $(BINDIR)
	cp $(TARGET) $(BINDIR)/$(TARGET)
	@echo "Installed $(TARGET) to $(BINDIR)"

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(BINDIR)"
