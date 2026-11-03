# Assignment 3 - Spell-check utility using 8086 string instructions (MASM)

Compares `CHECK_WORD` against each 10-byte dictionary entry using `REPE CMPSB`.
Prints `SPELLING CORRECT` or `SPELLING INCORRECT`.

Build in DOSBox:
```
MASM spellcheck.asm;
LINK spellcheck.obj;
spellcheck.exe
```
Code was written in `edit.com`. Screenshots in `images/`.
