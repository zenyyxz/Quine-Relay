#!/bin/bash
set -e

# Compile builder
g++ -O3 src/builder.cpp -o builder

# Run builder to generate relay.cpp
./builder

# 1. C++ -> Brainfuck
echo "C++ -> Brainfuck"
g++ -O3 src/relay.cpp -o relay_cpp
./relay_cpp > relay.bf

# 2. Brainfuck -> C
echo "Brainfuck -> C"
# Using a simple python-based BF interpreter for convenience
python3 -c "
import sys
code = open('relay.bf').read()
mem = [0] * 30000
ptr = 0
pc = 0
loops = []
bracket_map = {}
for i, c in enumerate(code):
    if c == '[': loops.append(i)
    if c == ']':
        start = loops.pop()
        bracket_map[start] = i
        bracket_map[i] = start
while pc < len(code):
    c = code[pc]
    if c == '>': ptr += 1
    elif c == '<': ptr -= 1
    elif c == '+': mem[ptr] = (mem[ptr] + 1) % 256
    elif c == '-': mem[ptr] = (mem[ptr] - 1) % 256
    elif c == '.': sys.stdout.write(chr(mem[ptr]))
    elif c == '[' and mem[ptr] == 0: pc = bracket_map[pc]
    elif c == ']' and mem[ptr] != 0: pc = bracket_map[pc]
    pc += 1
" > relay.c

# 3. C -> Python
echo "C -> Python"
gcc -O3 relay.c -o relay_c
./relay_c > relay.py

# 4. Python -> Assembly
echo "Python -> Assembly"
python3 relay.py > relay.asm

# 5. Assembly -> C++
echo "Assembly -> C++"
nasm -f elf64 relay.asm -o relay.o
ld relay.o -o relay_asm
./relay_asm > relay_final.cpp

# Compare
echo "Verifying..."
if diff src/relay.cpp relay_final.cpp; then
    echo "SUCCESS! The Ouroboros cycle is complete and byte-identical."
else
    echo "FAILURE: The cycle output differs from the original."
    exit 1
fi
