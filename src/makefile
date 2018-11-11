# the compiler
CC = gcc
CC++ = g++
# Compiler flags
CFLAGS = -g -std=gnu11 -Wall -Wextra

EXE = 537make

# Dir for clang static analyzer
SCAN_BUILD_DIR = ../scan_build/

SOURCES = build_spec.c command.c graph.c list.c main.c parser.c tools.c
OBJECTS = build_spec.o command.o graph.o list.o main.o parser.o tools.o

default: $(EXE)

$(EXE): $(OBJECTS)
	@$(CC) $(OBJECTS) $(CFLAGS) -o $(EXE)
	@echo "Linking Complete!"

build_spec.o: build_spec.c build_spec.h
	@$(CC) $(CFLAGS) -c build_spec.c
	@echo "Compiled build_spec!"

command.o: command.c command.h
	@$(CC) $(CFLAGS) -c command.c
	@echo "Compiled command!"

graph.o: graph.c graph.h
	@$(CC) $(CFLAGS) -c graph.c
	@echo "Compiled graph!"

list.o: list.c list.h
	@$(CC) $(CFLAGS) -c list.c
	@echo "Compiled list!"

main.o: main.c main.h
	@$(CC) $(CFLAGS) -c main.c
	@echo "Compiled main!"

parser.o:	parser.c parser.h
	@$(CC) $(CFLAGS) -c parser.c
	@echo "Compiled parser!"

tools.o:	tools.c tools.h
	@$(CC) $(CFLAGS) -c tools.c
	@echo "Compiled tools!"

.PHONY: clean
clean:
	@rm -f *.o *.a $(EXE)
	@rm -rf $(SCAN_BUILD_DIR)
	@echo "Cleaned!"

.PHONY: scan-build
scan-build: clean
	@scan-build -o $(SCAN_BUILD_DIR) make

.PHONY: scan-view
scan-view: scan-build
	@firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
