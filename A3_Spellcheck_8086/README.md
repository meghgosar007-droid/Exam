# Assignment 3: Spell-check utility using 8086 string instructions (MASM)

**Aim:** check a word against a small dictionary using the 8086 string instructions `CMPSB` and `REP`, and print `SPELLING CORRECT` or `SPELLING INCORRECT`.

Files in this folder:

| File | What it is |
| --- | --- |
| `spellcheck.asm` | The program from the lab (unchanged) |
| `spellcheck_fixed.asm` | Corrected version, see "A bug in the lab program" below |
| `images/` | Screenshots: `edit_com.png`, `masm_1.png`, `masm_2.png`, `masm_3.png` |

---

## Idea

Every dictionary word is padded with spaces to **10 bytes**, so every entry has the same length and entry *n* starts at `DICT + 10*n`. The word to check is padded the same way.

For each entry, compare 10 bytes of the entry (`DS:SI`) with the 10 bytes of `CHECK_WORD` (`ES:DI`) using `REPE CMPSB`. If all 10 match, the word is correct. If not, move to the next entry. If the end of the dictionary is reached, the word is incorrect.

Algorithm:

1. Point `DS` and `ES` at the data segment.
2. `SI` = start of the dictionary.
3. `DI` = `CHECK_WORD`, `CX` = 10, direction forward.
4. Compare up to 10 bytes while they are equal.
5. All 10 equal: print `SPELLING CORRECT` and stop.
6. Otherwise `SI` = start of the next entry. If `SI` is before the end of the dictionary, go to step 3.
7. Print `SPELLING INCORRECT` and stop.

## Step 1: Working folder and DOSBox

Use the folder from Assignment 1 (it needs `MASM.EXE`, `LINK.EXE`, `EDIT.COM`):

```
mount c c:\coa
c:
```

## Step 2: Type the program with EDIT

DOS file names are limited to 8 characters, so `spellcheck.asm` is stored as `SPELLCHE.ASM`. To avoid confusion, use a short name:

```
edit spell.asm
```

Type the program (screenshot: `images/edit_com.png`), or copy it from `spellcheck.asm`. Save with `Alt+F`, then `S`, and exit with `Alt+F`, then `X`.

## Step 3: Assemble

```
masm spell.asm;
```

The `;` accepts the default answers. Expect `0 Warning Errors` and `0 Severe Errors` (screenshots: `images/masm_1.png`, `images/masm_2.png`). This creates `SPELL.OBJ`.

## Step 4: Link

```
link spell.obj;
```

Expect `LINK : warning L4021: no stack segment`. It is harmless for this program. `spellcheck_fixed.asm` has a stack segment, so it does not show the warning. This creates `SPELL.EXE`.

## Step 5: Run

```
spell.exe
```

With `CHECK_WORD DB 'APPEL     '` the output is:

```
SPELLING INCORRECT
```

(screenshot: `images/masm_3.png`). `APPEL` is not in the dictionary, `APPLE` is.

## Step 6: Test other words

Change the word in the data segment and repeat steps 3 to 5. Keep it padded to exactly 10 characters:

| `CHECK_WORD` | Expected output |
| --- | --- |
| `'APPLE     '` | SPELLING CORRECT |
| `'APPEL     '` | SPELLING INCORRECT |
| `'MANGO     '` | SPELLING CORRECT |
| `'GRAPE     '` | SPELLING CORRECT |
| `'GRAPES    '` | SPELLING INCORRECT |

To add a word, add a line `DB 'KIWI      '` (padded to 10) before `DICT_END`. The end check uses `DICT_END`, so nothing else changes.

## Code explained

| Line | Meaning |
| --- | --- |
| `ASSUME CS: CODE, DS: DATA` | Tells the assembler which segment each register refers to |
| `DICT_END LABEL BYTE` | Marks the end of the dictionary, with no data of its own |
| `MSG_... DB '...$'` | Messages, ended with `$` as DOS function 09H requires |
| `MOV AX, DATA` / `MOV DS, AX` | Load the data segment address into `DS` |
| `MOV ES, AX` | `CMPSB` compares `DS:SI` with `ES:DI`, so `ES` must be the same segment |
| `LEA SI, DICT` | `SI` = first dictionary entry |
| `LEA DI, CHECK_WORD` | `DI` = word to check |
| `MOV CX, 10` | Compare at most 10 bytes |
| `CLD` | Direction flag = 0, so `SI` and `DI` go up |
| `REPE CMPSB` | Compare `[SI]` with `[DI]`, step both, repeat while equal and `CX` is not 0 |
| `ADD SI, 10` | Next entry |
| `CMP SI, OFFSET DICT_END` / `JB CHECK` | Loop while `SI` is still inside the dictionary |
| `MOV AH, 09H` / `INT 21H` | Print the `$`-terminated string at `DS:DX` |
| `MOV AH, 4CH` / `INT 21H` | Exit to DOS |

---

## A bug in the lab program

While checking the program in an 8086 emulator, I found two problems in `spellcheck.asm`. They do not show up with the test word `APPEL`, because that word is wrong and the first entry is the one it differs from.

**1. `SI` is not at the start of the next entry.** `REPE CMPSB` moves `SI` forward by the number of bytes compared, not by 10. If the words differ at the 3rd byte, `SI` has moved 3, and `ADD SI, 10` then lands 7 bytes before the true start of the next entry. Every entry after the first is compared from the wrong place, so a correct word such as `MANGO` is reported as `SPELLING INCORRECT`.

**2. `JCXZ FOUND` is not a reliable test.** If the words differ only in the last byte, `CX` also reaches 0, and the program wrongly reports the word as correct.

**Fix** (in `spellcheck_fixed.asm`): save `SI` with `PUSH SI` before `REPE CMPSB`, restore it with `POP SI` afterwards, and test the zero flag with `JE FOUND`. `ZF` is set only if all 10 bytes matched, and `POP` does not change flags.

```
        PUSH SI
        REPE CMPSB
        POP SI
        JE FOUND
```

Results in the emulator:

| `CHECK_WORD` | `spellcheck.asm` | `spellcheck_fixed.asm` |
| --- | --- | --- |
| `APPLE` | CORRECT | CORRECT |
| `APPEL` | INCORRECT | INCORRECT |
| `MANGO` | INCORRECT (wrong) | CORRECT |
| `GRAPE` | INCORRECT (wrong) | CORRECT |

`spellcheck_fixed.asm` was checked in the emulator and was not assembled with MASM in this session. Run `masm` and `link` on it in DOSBox before relying on it.

If your lab marking is on the original, keep `spellcheck.asm` as submitted and mention the fix as an extra.

## Conclusion

Learned to use the string instructions `CMPSB` with `REPE`, the `ES`/`DS` roles in string comparison, and to build and run an 8086 program with `MASM` and `LINK`.
