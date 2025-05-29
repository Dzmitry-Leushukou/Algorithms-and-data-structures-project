import time
import bisect
import psutil
import struct
import os
from pathlib import Path

def get_current_memory():
    process = psutil.Process()
    return process.memory_info().rss

def min_deletions_for_increasing_sequence(A):
    if not A:
        return 0
    tail = []
    for x in A:
        idx = bisect.bisect_left(tail, x)
        if idx == len(tail):
            tail.append(x)
        else:
            tail[idx] = x
    return len(A) - len(tail)

def read_binary_test_file(filepath):
    """Read binary file containing test data"""
    with open(filepath, 'rb') as f:
        # Read 4-byte integers (assuming the files contain int32 values)
        data = f.read()
        # Convert bytes to list of integers
        # Using struct to unpack binary data (assuming little-endian 32-bit integers)
        int_size = 4
        count = len(data) // int_size
        fmt = '<' + 'i' * count  # '<' for little-endian, 'i' for 4-byte integer
        return list(struct.unpack(fmt, data))

def process_test_file(filepath):
    """Process a single test file and print results"""
    # Get test parameters from filename
    filename = Path(filepath).name
    parts = filename.split('_')
    size = int(parts[1])
    test_type = int(parts[2].split('.')[0])
    
    # Read test data
    A = read_binary_test_file(filepath)
    
    # Get initial memory state
    mem_before = get_current_memory()
    
    # Run algorithm
    start_time = time.time()
    result = min_deletions_for_increasing_sequence(A)
    end_time = time.time()
    
    # Get final memory state
    mem_after = get_current_memory()
    
    # Calculate memory usage
    memory_used_bytes = (mem_after - mem_before) if (mem_after > mem_before) else 0
    memory_used_mb = memory_used_bytes / (1024 * 1024)
    bytes_per_element = memory_used_bytes / size if size > 0 else 0
    
    # Calculate execution time
    duration_ms = (end_time - start_time) * 1000
    
    # Print results
    print(f"Processing file: {filename}")
    print(f"Size: {size}, Type: {test_type}")
    print(f"Result: {result}")
    print(f"Time: {duration_ms:.2f} ms")
    print(f"Memory: {memory_used_mb:.2f} MB")
    print(f"Bytes per element: {bytes_per_element:.2f}")
    print("-" * 50)

def main():
    # Base directory containing test files
    base_dir = r"C:\Users\User\Desktop\aisf\datasets\9"
    
    # List of test files to process (you can modify this list as needed)
    test_files = [
        "test_10000_0.bin",
        "test_10000_1.bin",
        "test_10000_2.bin",
        "test_10000_3.bin",
        "test_10000_4.bin",
        "test_100000_0.bin",
        "test_100000_1.bin",
        "test_100000_2.bin",
        "test_100000_3.bin",
        "test_100000_4.bin",
        "test_1000000_0.bin",
        "test_1000000_1.bin",
        "test_1000000_2.bin",
        "test_1000000_3.bin",
        "test_1000000_4.bin"
    ]
    
    print("Testing min_deletions_for_increasing_sequence algorithm")
    print("=" * 50)
    
    for test_file in test_files:
        filepath = os.path.join(base_dir, test_file)
        if os.path.exists(filepath):
            process_test_file(filepath)
        else:
            print(f"File not found: {filepath}")
    
    print("Testing completed!")

if __name__ == "__main__":
    main()