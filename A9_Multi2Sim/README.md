# Assignment 9 - Parallel computing architectures with Multi2Sim

32x32 matrix multiplication in functional and detailed mode, then 1/2/4/8-core x86 systems
(private 8 KB L1 per core, shared 128 KB L2, one main memory).

Run (inside the `docker.io/multi2sim/multi2sim` container, repo mounted at `/work`):
```
mkdir -p exe out reports config
./pod/create && ./pod/start
gcc -m32 -static -O0 -std=gnu99 code/matmul.c -o exe/matmul
m2s --x86-sim functional ./exe/matmul
m2s --x86-sim detailed   ./exe/matmul
./scripts/singlecore.sh
./scripts/multicore.sh
```

## Results (from the report)
Functional: 986,179 instr, 0.19 s. Detailed: 1,085,196 instr, 1,843,943 cycles, IPC 0.5346, 29.71 s.

| Cores | Instructions | Cycles  | IPC   | L1 hit | L2 hit | Speedup | Efficiency |
|------:|-------------:|--------:|------:|-------:|-------:|--------:|-----------:|
| 1 | 1,116,874 | 832,203 | 1.187 | 0.9764 | 0.4383 | 1.00 | 1.00 |
| 2 | 1,104,115 | 554,070 | 1.785 | 0.9780 | 0.4444 | 1.50 | 0.75 |
| 4 | 1,109,996 | 406,226 | 2.440 | 0.9770 | 0.4510 | 2.05 | 0.51 |
| 8 | 1,123,816 | 341,727 | 2.921 | 0.9751 | 0.4630 | 2.44 | 0.30 |

Amdahl serial fraction from the 8-core speedup:
f = (1/S - 1/n) / (1 - 1/n) = (1/2.435 - 1/8) / (1 - 1/8) = 0.326, so max speedup = 1/f = 3.06.
