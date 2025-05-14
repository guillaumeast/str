# Compiler
CXX      	?= clang++
CXXFLAGS 	:= -std=c++17 -Wall -Wextra -pedantic -Iinclude -I/opt/homebrew/include
LDFLAGS  	:= -L/opt/homebrew/lib -lunistring

BUILD_DIR 	:= build
TARGET 		:= str
OUTPUT    	:= $(BUILD_DIR)/$(TARGET)
SRC    		:= $(wildcard src/*.cpp)

# Build
$(OUTPUT): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean

# Install
PREFIX  ?= $(HOME)/.local
BINDIR  := $(PREFIX)/bin

install: $(OUTPUT)
	mkdir -p $(BINDIR)
	cp $(OUTPUT) $(BINDIR)/$(TARGET)
	@echo "Installed $(TARGET) to $(BINDIR)"

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	@echo "Uninstalled $(TARGET) from $(BINDIR)"

