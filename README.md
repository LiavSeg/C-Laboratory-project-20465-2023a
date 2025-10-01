# C Laboratory Project – 20465 (2023a) ![Views](https://visitor-badge.laobi.icu/badge?page_id=LiavSeg.C-Laboratory-project-20465-2023a)



Assembler and macro preprocessor written in C as part of the Open University course 20465 – Systems Programming Lab.
Implements a two-pass assembler for a hypothetical machine with 14-bit instructions and a limited instruction set, including full support for macros, symbol resolution, and error diagnostics.

**Language**: C (GCC)  
**Architecture**: Two-pass assembler  
**Grade**: 94

**Features:** symbol table generation, macro expansion (.macro/.endmacro), label handling and address resolution, instruction encoding into binary format, full error reporting (syntax and semantic), modular design.

**Target Machine:** 8 general-purpose registers, 14-bit words.

---

## ⚙️ Build Instructions

To compile and run the assembler:

```bash
git clone https://github.com/LiavSeg/C-Laboratory-project-20465-2023a.git
cd C-Laboratory-project-20465-2023a
make
./assembler examples/example1.as
```

The build process will compile the assembler and place the binary in the root directory (likely named `assembler`).

---

##  Usage

Once built, the assembler can be run from the terminal with `.as` source files.  
Expected output files include:

- `.ob` – Encoded machine code  
- `.ent` – Entry labels  
- `.ext` – External labels  

Error messages will be printed to the terminal.

---

##  Features In-Depth

**First Pass:**

- Detects labels, instructions, directives  
- Builds symbol table  
- Parses macro definitions  
- Handles `.data`, `.string`, `.extern`, `.entry`  

**Second Pass:**

- Resolves symbols to addresses  
- Encodes each instruction to binary  
- Detects undefined symbols and conflicting labels  
- Produces output files  

**Preprocessor:**

- Expands user-defined macros before assembly  
- Supports nested macros  
- Replaces macro calls with their content inline  

---

##  Example Input

```asm
MAIN:   mov r3, LENGTH
        add r2, r3
        .macro m1
        sub r1, r4
        .endmacro
        m1
LENGTH: .data 6,-9,15
```

After preprocessing and two-pass processing, output files will contain encoded binary and symbol references.

---

**Author**: Liav Segev 
**GitHub Profile**: https://github.com/LiavSeg
