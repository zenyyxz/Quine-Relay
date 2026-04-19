# Ouroboros: A 5-Language Quine Relay

A byte-perfect Quine Relay (Ouroboros program) that traverses five distinct execution environments before returning to its original source code. This project was built from scratch to demonstrate low-level string escaping, programmatic code generation, and cross-language data persistence.

## The Cycle

The relay executes in the following order, with each stage generating the exact source code for the next:

1.  **C++** $\to$ **Brainfuck**
2.  **Brainfuck** $\to$ **C**
3.  **C** $\to$ **Python**
4.  **Python** $\to$ **Assembly (x86_64 NASM)**
5.  **Assembly** $\to$ **C++** (Closing the loop)

## Technical Architecture

### 1. The Universal Payload (`S`)
The core of the relay is a universal data string `S` embedded in every stage. To prevent parsing collisions and "escape hell," we utilize a non-printable ASCII `SOH` (`\1`) character as a delimiter. This string contains the "blueprints" (boilerplates) for all five languages.

### 2. Stage Implementation Highlights

*   **C++ (The Generator):** Implements a programmatic Brainfuck engine. It constructs the target C source in memory and translates every byte into optimized Brainfuck pointer movements and increments.
*   **Brainfuck (The Esoteric Bridge):** A raw pointer-based program that, when interpreted, prints the C stage source. It serves as a pure logic test between the compiled stages.
*   **C (The Byte Manipulator):** Uses raw `char*` pointer arithmetic and `fwrite` to extract the Python stage. It handles data persistence by hex-escaping the universal payload to ensure no byte information is lost during the transition.
*   **Python (The Formatter):** Parsed as a high-level script, it reads the universal payload and formats it into a series of NASM `db` (define byte) sequences, effectively bridging the gap between high-level scripting and raw machine instructions.
*   **Assembly (The Flex):** A pure x86_64 Linux Assembly program (NASM). It defines no external dependencies and uses raw Linux syscalls (`sys_write`, `sys_exit`). It manually traverses the `.data` section to find the Nth delimiter and prints the original C++ code.

## Verification

The relay is verified through a strict byte-for-byte comparison. The `test.sh` script automates the entire compilation and execution pipeline:

```bash
# Compilation & Execution Pipeline
g++ (C++) -> bf-interpreter -> gcc (C) -> python3 -> nasm/ld (ASM) -> diff
```

A successful run concludes with:
`SUCCESS! The Ouroboros cycle is complete and byte-identical.`

## Project Structure

*   `src/builder.cpp`: The "Architect." A C++ tool that assembles the boilerplates and initializes the first `relay.cpp`.
*   `src/relay.cpp`: The entry point of the cycle.
*   `src/test.sh`: The automated validation suite.

## Requirements

*   `g++` / `gcc`
*   `python3`
*   `nasm`
*   `ld` (binutils)
*   A Linux environment (for x86_64 syscall compatibility)
