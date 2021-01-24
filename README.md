# External_sort

## General idea
* Load a part of the input that can fit into RAM to RAM then sort it and output them into N files
* Perform N-way merge those N files into 1 output file. For example if we have 1GB RAM and 9 500MB sorted files, we will load 900MB into a minHeap (100MB each file) and that will leave us with 100MB left for output. 
* Pop that heap into output file, whenever we finish pop 100MB from any file, load another 100MB into our minHeap until we done with all of them
* When we used up 100MB for output, we'll flush the sorted data to our output file to make room for other data
# How to use

Program accept input file name, output file name and memory limit as parameters

> ➜ g++ main.cpp -o main -std=c++14 && ./main "input_name" "output_name" memory_limit
