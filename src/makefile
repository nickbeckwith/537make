# the compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra

EXE = 537make

# Dir for clang static analyzer
SCAN_BUILD_DIR = ../scan_build/

SOURCES = build_spec.c parser.c tools.c
OBJECTS = build_spec.o parser.o tools.o

default: $(EXE)

$(EXE): $(OBJECTS)
	@$(CC) $(OBJECTS) $(CFLAGS) -o $(EXE)
	@echo "Linking Complete!"

build_spec.o: build_spec.c build_spec.h
	@$(CC) $(CFLAGS) -c build_spec.c
	@echo "Compiled build_spec!"

parser.o:	parser.c parser.h
	@$(CC) $(CFLAGS) -c parser.c
	@echo "Compiled parser!"

tools.o:	tools.c tools.h
	@$(CC) $(CFLAGS) -c tools.c
	@echo "Compiled tools!"

.PHONY: clean
clean:
	@rm -f *.o $(EXE)
	@rm -rf $(SCAN_BUILD_DIR)
	@echo "Cleaned!"

.PHONY: scan-build
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

.PHONY: scan-view
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
