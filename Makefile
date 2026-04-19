# Makefile for Ouroboros Quine Relay

CXX = g++
CXXFLAGS = -O3 -Wall
NASM = nasm
ASFLAGS = -f elf64
LD = ld

all: test

# Initialize the first relay file from the builder
src/relay.cpp: src/builder.cpp
	$(CXX) $(CXXFLAGS) src/builder.cpp -o builder
	./builder
	rm -f builder

# Run the full automated cycle
test: src/relay.cpp src/test.sh
	@bash src/test.sh

clean:
	rm -f builder relay_cpp relay.bf relay_c relay.c relay.py relay.asm relay.o relay_asm relay_final.cpp

.PHONY: all test clean
