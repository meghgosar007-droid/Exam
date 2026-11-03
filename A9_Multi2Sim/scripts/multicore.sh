#!/bin/bash
echo "----- MULTI CORE -----"
for n in 1 2 4 8; do
    gcc -m32 -static -O0 -std=gnu99 -pthread -DTHREADS=$n code/mt.c -o exe/mt$n
    cat > config/x86_$n.cfg <<EOF
[ General ]
Cores = $n
Threads = 1
EOF
    ./scripts/gen_mem.sh $n > config/mem_$n.cfg
    m2s --x86-sim detailed --x86-config config/x86_$n.cfg --mem-config config/mem_$n.cfg \
        --mem-report reports/mem_$n.rpt ./exe/mt$n > out/$n.txt 2>&1
    echo "== $n cores =="; grep -E "Instructions|Cycles|IPC|Cache|Warning:|Panic:" out/$n.txt || true
done
