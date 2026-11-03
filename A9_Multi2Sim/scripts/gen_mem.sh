#!/bin/bash
# Usage: ./gen_mem.sh <cores> > mem_<cores>.cfg
N=$1
cat <<EOF
[CacheGeometry geo-l1]
Sets = 64
Assoc = 2
BlockSize = 64
Latency = 2
Policy = LRU
Ports = 2

[CacheGeometry geo-l2]
Sets = 512
Assoc = 4
BlockSize = 64
Latency = 10
Policy = LRU
Ports = 4

[Module mod-l2]
Type = Cache
Geometry = geo-l2
HighNetwork = net-l1-l2
LowNetwork = net-l2-mm
LowModules = mod-mm

[Module mod-mm]
Type = MainMemory
BlockSize = 64
Latency = 100
HighNetwork = net-l2-mm

[Network net-l1-l2]
DefaultInputBufferSize = 1024
DefaultOutputBufferSize = 1024
DefaultBandwidth = 256

[Network net-l2-mm]
DefaultInputBufferSize = 1024
DefaultOutputBufferSize = 1024
DefaultBandwidth = 256
EOF

for ((c=0; c<N; c++)); do
cat <<EOF

[Module mod-l1-$c]
Type = Cache
Geometry = geo-l1
LowNetwork = net-l1-l2
LowModules = mod-l2

[Entry core$c]
Arch = x86
Core = $c
Thread = 0
DataModule = mod-l1-$c
InstModule = mod-l1-$c
EOF
done
