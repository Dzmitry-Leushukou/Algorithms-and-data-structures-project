import time
import psutil
from bst import BST, FileService

def process_file(filename):
    start_time = time.perf_counter()
    process = psutil.Process()
    
    tree = BST()
    fs = FileService(tree, filename)
    fs.read()

    tree.solve_task3()

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Processing file: {filename}")
    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Mem: {peak_memory_mb:.2f} MB")
    print()

def main():
    dataset_files = [
    "./datasets/3/10000.txt",
    "./datasets/3/10000_2.txt",
    "./datasets/3/100000.txt",
    "./datasets/3/1000000.txt"
]

    for filename in dataset_files:
        process_file(filename)

if __name__ == "__main__":
    main()