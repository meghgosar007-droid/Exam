# Assignment 2: COPY / PASTE (block transfer) in 8086 using DEBUG

**Aim:** simulate copy and paste of a block of memory using the DEBUG utility. The program must work in every case, including when the source and destination blocks overlap.

---

## The idea

Copy `CX` bytes from the area at `SI` (source) to the area at `DI` (destination).

The direction of the copy matters when the two areas overlap:

| Case | What to do | Why |
| --- | --- | --- |
| `SI == DI` | Nothing to copy (copying onto itself is harmless) | Same place |
| `DI < SI` (destination below source) | Copy **forward**, first byte to last | Bytes are copied before they get overwritten |
| `DI > SI` (destination above source) | Copy **backward**, last byte to first | A forward copy would overwrite source bytes that are not copied yet |

Algorithm (generalised):

1. Initialise the counter (`CX`), source pointer (`SI`) and destination pointer (`DI`).
2. Compare `DI` with `SI`.
   - If `DI > SI`, go to step 5 (backward copy).
   - Otherwise continue with the forward copy.
3. **Forward copy:** read the byte at `[SI]`, write it to `[DI]`, increment `SI` and `DI`, decrement `CX`. Repeat while `CX` is not 0.
4. Stop.
5. **Backward copy:** move both pointers to the last byte (`SI = SI + CX - 1`, `DI = DI + CX - 1`).
6. Read the byte at `[SI]`, write it to `[DI]`, decrement `SI` and `DI`, decrement `CX`. Repeat while `CX` is not 0.
7. Stop.

> The simple block transfer (no direction check) is steps 3 and 4 only. It fails for overlapping blocks with `DI > SI`.

---

## Step-by-step in DEBUG

### Step 1: Start DEBUG

Start DOSBox as in Assignment 1 (`mount c ...`, `c:`), then:

```
debug
```

### Step 2: Type the program

Type `a100` and then these lines, one per line, pressing Enter after each. DEBUG prints the address in front of each line by itself, so you only type the instruction.

```
-a100
073F:0100 MOV AX,1000
073F:0103 MOV DS,AX
073F:0105 MOV SI,0300
073F:0108 MOV DI,0400
073F:010B MOV CX,000A
073F:010E CMP DI,SI
073F:0110 JG 011D
073F:0112 MOV AL,[SI]
073F:0114 MOV [DI],AL
073F:0116 INC SI
073F:0117 INC DI
073F:0118 DEC CX
073F:0119 JNZ 0112
073F:011B JMP 012C
073F:011D ADD SI,CX
073F:011F DEC SI
073F:0120 ADD DI,CX
073F:0122 DEC DI
073F:0123 MOV AL,[SI]
073F:0125 MOV [DI],AL
073F:0127 DEC SI
073F:0128 DEC DI
073F:0129 DEC CX
073F:012A JNZ 0123
073F:012C INT 3
073F:012D
```

Press Enter on the empty line (`073F:012D`) to leave assemble mode. Screenshots of the listing: `images/code_listing.png` and `images/code_2.png`.

Check what you typed with `u100 12c`. The addresses must match the listing above. If a jump address is wrong (`JG 011D`, `JNZ 0112`, `JMP 012C`, `JNZ 0123`), one of the earlier instructions was typed wrongly.

### Step 3: Put test data in the source area

```
-E 1000:0300 11 22 33 44 55 66 77 88 99 AA
```

This writes 10 bytes at segment `1000`, offset `0300`.

### Step 4: Run it

```
-G=100
```

Expected (screenshot: `images/output.png`):

```
AX=1011  BX=0000  CX=0000  DX=0000  SP=00FD  BP=0000  SI=02FF  DI=03FF
DS=1000  ES=073F  SS=073F  CS=073F  IP=012C   NV UP EI PL ZR NA PE NC
073F:012C CC            INT     3
```

`CX=0000` means all 10 bytes were copied. `SI=02FF` and `DI=03FF` show that the **backward** path ran, because `DI` (0400) is above `SI` (0300). Both pointers moved down by 10.

### Step 5: Check the destination

```
-D 1000:0300 L A
1000:0300  11 22 33 44 55 66 77 88-99 AA
-D 1000:0400 L A
1000:0400  11 22 33 44 55 66 77 88-99 AA
```

Both areas hold the same 10 bytes, so the copy worked.

---

## Line-by-line explanation

| Address | Instruction | Meaning |
| --- | --- | --- |
| 0100 | `MOV AX,1000` | AX = data segment value |
| 0103 | `MOV DS,AX` | DS = 1000, so `[SI]` and `[DI]` use segment 1000 |
| 0105 | `MOV SI,0300` | Source offset |
| 0108 | `MOV DI,0400` | Destination offset |
| 010B | `MOV CX,000A` | Count = 10 bytes |
| 010E | `CMP DI,SI` | Compare destination with source |
| 0110 | `JG 011D` | If `DI > SI`, jump to the backward copy |
| 0112 to 0119 | forward loop | Copy `[SI]` to `[DI]`, `INC SI`, `INC DI`, `DEC CX`, repeat while `CX` is not 0 |
| 011B | `JMP 012C` | Forward copy finished, jump to the end |
| 011D to 0122 | pointer setup | `SI = SI + CX - 1` and `DI = DI + CX - 1`, so both point at the last byte |
| 0123 to 012A | backward loop | Copy `[SI]` to `[DI]`, `DEC SI`, `DEC DI`, `DEC CX`, repeat while `CX` is not 0 |
| 012C | `INT 3` | Stop, DEBUG shows the registers |

## Try the other cases

The source and destination offsets live in the instructions at `0105` and `0108`, and the count at `010B`. To change them, re-assemble just those lines:

```
-a105
073F:0105 mov si,0400
073F:0108 mov di,0300
073F:010B mov cx,000a
073F:010E
```

Press Enter on the empty line to stop. Then put data at the new source with `E`, run with `G=100`, and look at the destination with `D`.

| Test | `SI` | `DI` | Path taken | Put data at | Look at |
| --- | --- | --- | --- | --- | --- |
| Lab case | 0300 | 0400 | backward | `1000:0300` | `D 1000:0400 L A` |
| Destination below source | 0400 | 0300 | forward | `1000:0400` | `D 1000:0300 L A` |
| Overlap, destination above | 0300 | 0305 | backward | `1000:0300` | `D 1000:0300 L F` |
| Overlap, destination below | 0305 | 0300 | forward | `1000:0305` | `D 1000:0300 L F` |
| Same address | 0300 | 0300 | forward | `1000:0300` | `D 1000:0300 L A` |

For the overlap case with `SI=0300` and `DI=0305`, the 15 bytes at `1000:0300` end up as
`11 22 33 44 55 11 22 33 44 55 66 77 88 99 AA`. The first five bytes are the untouched start of the source, and the next ten are the copy. A forward copy would have given the wrong answer here, which is why the direction check exists.

## Run it from a script (optional)

```
debug < scripts\block_transfer.txt
debug < scripts\block_transfer_overlap.txt
```

`block_transfer.txt` is the lab case (Steps 2 to 5). `block_transfer_overlap.txt` is the overlap test (`SI=0300`, `DI=0305`).

## Checking the results

The program above was assembled at the exact addresses in the listing and run in an 8086 emulator for the lab case and for these cases: destination below source, both overlaps, same address, a copy of 5 bytes and a copy of 1 byte. Every case copied the data correctly, and the lab case ended with `SI=02FF` and `DI=03FF`, as in the screenshot.

## Conclusion

A block copy is only correct for overlapping memory if it picks its direction: copy forward when the destination is below the source, and backward when it is above.
