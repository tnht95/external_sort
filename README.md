# External Sort

This repository contains an implementation of an **External Sort**, a technique designed to efficiently sort data sets that exceed the capacity of system memory. The implementation utilizes **in-memory sorting** combined with **N-way merging** to minimize memory usage and handle large input sizes effectively.

## General Idea

1. **Chunk-Based Sorting**:
   - The input is divided into chunks that fit into available RAM.
   - Each chunk is sorted in-memory and written to intermediate files on disk.

2. **N-Way Merge**:
   - All sorted chunks are merged into a single sorted output file.
   - A min-heap is used to manage the smallest elements from each chunk efficiently.
   - When the heap's output buffer reaches capacity, the system flushes the sorted data to disk, optimizing I/O efficiency.

### Example

If the system has 1GB of RAM and 9 files, each 500MB:
- Load 900MB into memory (100MB per file, leaving 100MB for output).
- Use a min-heap to merge these 9 sorted files.
- Flush the sorted 100MB buffer to the output file as needed.
- Repeat until all files are processed.

## Key Features

- **Efficient Memory Usage**: Optimized for sorting data sets larger than available memory.
- **Min-Heap-Based Merging**: Ensures minimal overhead during the N-way merge.
- **Buffered Output**: Writes sorted data incrementally to reduce disk I/O operations.

## Project Structure

- **Chunk Loader**: Reads and sorts data chunks that fit into memory.
- **Min-Heap Merging**: Combines sorted chunks using a priority queue.
- **Output Buffer**: Manages incremental writes to the output file.

## Getting Started

### Building and Running

1. Clone the repository:
   ```bash
   git clone https://github.com/username/external-sort.git
   cd external-sort
   ```

2. Run the program
   
   - Program accept input file name, output file name and memory limit as parameters
  
     
   ```
   g++ main.cpp -o main -std=c++14 && ./main "input_name" "output_name" memory_limit
   ```
