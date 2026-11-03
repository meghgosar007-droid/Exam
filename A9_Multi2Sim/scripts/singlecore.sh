#!/bin/bash
echo "----- SINGLE CORE -----"
gcc -m32 -static -O0 -std=gnu99 -pthread -DTHREADS=1 code/mt.c -o exe/mt1
cat > config/x86_1.cfg <<EOF
[ General ]
Cores = 1
Threads = 1
EOF
./scripts/gen_mem.sh 1 > config/mem_1.cfg
m2s --x86-sim detailed --x86-config config/x86_1.cfg --mem-config config/mem_1.cfg \
    --mem-report reports/mem_1.rpt ./exe/mt1 > out/1.txt 2>&1
echo "== 1 core =="; grep -E "Instructions|Cycles|IPC|Cache|Warning:|Panic:" out/1.txt || true
