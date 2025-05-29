import time
import psutil
from bst import BST, FileService

def process_file(filename):
    start_time = time.perf_counter()
    process = psutil.Process()
    
    tree1 = BST()
    tree2 = BST()
    fs1 = FileService(tree1, filename)
    fs2 = FileService(tree2, filename)
    fs1.read()
    fs2.read()

    BST.can_be_obtained(tree1, tree2)

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Processing file: {filename}")
    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Mem: {peak_memory_mb:.2f} MB")
    print()

def main():
    dataset_files = [
    "./datasets/5/100.txt",
    "./datasets/5/100_1.txt",
    "./datasets/5/1000.txt",
    "./datasets/5/1000_1.txt",
    "./datasets/5/10000.txt",
    "./datasets/5/10000_2.txt",
    "./datasets/5/10000_1.txt"
]

    for filename in dataset_files:
        process_file(filename)

if __name__ == "__main__":
    main()