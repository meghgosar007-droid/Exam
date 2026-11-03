# Assignment 1 - DOSBox virtual environment & DEBUG arithmetic

Learned to use DOSBox and the DEBUG utility to run 8086 arithmetic.

Program traced in DEBUG (see `images/flowchart.png`):

```
MOV AX, 0003H
MOV BX, 0004H
ADD AX, BX      ; AX = 0007H
INT 3           ; breakpoint / stop
```

Screenshots of DOSBox setup and the DEBUG session are in `images/`.
