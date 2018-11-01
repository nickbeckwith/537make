# the compiler
CC = gcc

# Compiler flags
FLAGS = -g -Wall -Wextra
EXE = 537make

# Dir for clang static analyzer
SCAN_BUILD_DIR = out/scan_build/

SRCDIR = src
INCLDIR = includes
OUTDIR = out

# Grab source files
SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLDIR)/*.h)
OBJECTS := $(patsubst %.c, $(OUTDIR)/%.o, $(SOURCES))

default: $(OUTDIR)/$(EXE) 

$(OUTDIR)/$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) -o $(OUTDIR)/$(EXE)
	echo "Linking complete!"

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(FLAGS) -c $(input) -o $(output)

.PHONY: clean
clean:
	@rm -f $(OUTDIR)/*.o $(OUTDIR)/$(EXE)
	@rm -rf $(SCAN_BUILD_DIR)
	@echo "Cleaned out"

.PHONY: scan-build
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

# View Clang Static
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
	
