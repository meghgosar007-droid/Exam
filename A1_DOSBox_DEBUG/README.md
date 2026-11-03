# Assignment 1: DOSBox virtual environment and DEBUG arithmetic

**Aim:** create a DOS environment with DOSBox and use the DEBUG utility to run 8086 arithmetic (add, subtract) by typing assembly directly into memory.

DEBUG shows every register after the program runs, so you can see exactly what each instruction did.

> All numbers typed into DEBUG are **hexadecimal**. `0003` means 3, `0017` means 23.

---

## What you need

| Item | Where it comes from |
| --- | --- |
| DOSBox 0.74 (or newer) | Installed on your computer |
| `DEBUG.EXE` | The lab's MASM/DOS folder |
| `MASM.EXE`, `LINK.EXE`, `EDIT.COM` | Same folder (needed from Assignment 3 onwards) |

---

## Step 1: Make the working folder

1. Create a folder, for example `C:\COA` (Windows) or `~/coa` (Linux/Mac).
2. Copy `DEBUG.EXE`, `MASM.EXE`, `LINK.EXE` and `EDIT.COM` into it.

## Step 2: Start DOSBox and mount the folder

Open DOSBox and type (screenshot: `images/dosbox_2.png`):

```
mount c c:\
c:
cd coa
```

You land in `C:\COA>`. DOSBox prints a red warning, `Mounting c:\ is NOT recommended`. It is harmless.

A cleaner way is to mount the folder itself, so no `cd` is needed:

```
mount c c:\coa
c:
```

On Linux or Mac use `mount c ~/coa`.

Useful DOS commands (screenshot: `images/dosbox_1.png`):

| Command | Use | Example |
| --- | --- | --- |
| `MOUNT C ~/` | Make a folder the C: drive | `MOUNT C ~/DESKTOP/8086` |
| `DIR` | List files and folders | `DIR` |
| `MASM` | Assemble an `.asm` source file | `MASM PROGRAM.ASM` |
| `LINK` | Make an `.exe` from the `.obj` file | `LINK PROGRAM.OBJ` |
| `DEBUG` | Start the DEBUG utility | `DEBUG PROGRAM.EXE` |
| `EXIT` | Leave the command environment | `EXIT` |

## Step 3: Learn the DEBUG commands used

| Command | Meaning |
| --- | --- |
| `a100` | **A**ssemble: type instructions starting at offset 100. A blank line ends it |
| `g=100` | **G**o: run from offset 100 until the `INT 3` breakpoint |
| `r` | Show the registers |
| `t=100` then `t` | **T**race: run one instruction at a time (use this to watch each step) |
| `u100` | **U**nassemble: list the instructions in memory |
| `d` | **D**ump memory |
| `q` | **Q**uit DEBUG |

`INT 3` is the breakpoint: it stops the program so DEBUG can print the registers.

---

## Step 4: Program 1, addition (3 + 4)

Start DEBUG:

```
debug
```

Then type exactly this. The `-` is DEBUG's prompt, and you press Enter after every line.

```
-a100
073F:0100 mov ax,0003
073F:0103 mov bx,0004
073F:0106 add ax,bx
073F:0108 int 3
073F:0109
-g=100
```

(The `073F` segment number will differ on your PC. After the last instruction, press Enter on an empty line to leave assemble mode.)

Expected result (screenshot: `images/debug_1.png`):

```
AX=0007  BX=0004  CX=0000  DX=0000  SP=00FD  BP=0000  SI=0000  DI=0000
DS=073F  ES=073F  SS=073F  CS=073F  IP=0108   NV UP EI PL NZ NA PO NC
073F:0108 CC            INT     3
```

**What each line does**

| Line | Meaning | Effect |
| --- | --- | --- |
| `mov ax,0003` | Load 3 into AX | AX = 0003 |
| `mov bx,0004` | Load 4 into BX | BX = 0004 |
| `add ax,bx` | AX = AX + BX | AX = 0007 |
| `int 3` | Stop | IP = 0108 |

Flow of the program: `images/flowchart.png`.

## Step 5: Program 2, subtraction (5 - 2)

Still inside DEBUG:

```
-a100
073F:0100 mov ax,0005
073F:0103 mov bx,0002
073F:0106 sub ax,bx
073F:0108 int 3
073F:0109
-g=100
```

Expected result (screenshot: `images/debug_2.png`):

```
AX=0003  BX=0002  CX=0000  DX=0000  SP=00FD  BP=0000  SI=0000  DI=0000
DS=073F  ES=073F  SS=073F  CS=073F  IP=0108   NV UP EI PL NZ NA PE NC
```

`AX = 0003`, which is 5 - 2.

## Step 6 (extra, not in the lab report): multiplication and division

```
-a100
073F:0100 mov ax,0006
073F:0103 mov bx,0004
073F:0106 mul bx
073F:0108 int 3
073F:0109
-g=100
```

`MUL BX` multiplies AX by BX and puts the 32-bit result in `DX:AX`. Expected: `AX=0018 DX=0000` (6 x 4 = 24 = 0x18).

```
-a100
073F:0100 mov ax,0017
073F:0103 mov bl,05
073F:0105 div bl
073F:0107 int 3
073F:0108
-g=100
```

`DIV BL` divides AX by BL. The **quotient goes in AL** and the **remainder in AH**. Expected: `AX=0304`, so quotient 4 and remainder 3 (23 / 5 = 4 remainder 3).

> The flag letters after `MUL` and `DIV` are not meaningful on the 8086, so only look at the registers.

## Step 7: Reading the flag letters

The last line of the register dump shows the flags:

| Flag | Set / clear shown as | Meaning when set |
| --- | --- | --- |
| Overflow | `OV` / `NV` | Signed overflow |
| Direction | `DN` / `UP` | String instructions go backward / forward |
| Interrupt | `EI` / `DI` | Interrupts enabled / disabled |
| Sign | `NG` / `PL` | Result negative / positive |
| Zero | `ZR` / `NZ` | Result is zero / not zero |
| Auxiliary carry | `AC` / `NA` | Carry out of bit 3 |
| Parity | `PE` / `PO` | Even / odd number of 1 bits in the low byte |
| Carry | `CY` / `NC` | Carry out of the top bit |

Check with the screenshots: `0007` = `0000 0111` has three 1 bits (odd), so `PO`. `0003` = `0000 0011` has two 1 bits (even), so `PE`.

## Step 8: Run a program from a script (optional)

The `scripts/` folder has the same programs as DEBUG input files. From the DOS prompt:

```
debug < scripts\add.txt
debug < scripts\sub.txt
debug < scripts\mul.txt
debug < scripts\div.txt
```

Each one assembles the program, runs it with `g=100`, prints the registers and quits.

---

## Checking the results

The four programs were also run in an 8086 emulator and gave the same values: add `AX=0007`, subtract `AX=0003`, multiply `AX=0018`, divide `AX=0304`.

## Conclusion

Learned to use DOSBox and the DEBUG utility, and to write assembly code that performs arithmetic on the 8086.
