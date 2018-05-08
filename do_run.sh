#!/bin/bash
vsizes=(64 256 1024 4096)
for vsize in ${vsizes[@]}; do
   
   #echo ${VALUE_SIZE}
   sudo VALUE_SIZE=${vsize} ./run_bench.sh | tee ./test_result/wskey_value$VALUE_SIZE 
done
