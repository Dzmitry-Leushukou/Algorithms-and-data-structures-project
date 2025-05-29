import time
import psutil

def process_file(filename):
    start_time = time.perf_counter()
    process = psutil.Process()
    
    try:
        with open(filename, 'r') as f:
            s1 = f.readline().strip()
            s2 = f.readline().strip()
            result = s1 == s2[::-1]
            print(f"Processing file: {filename}")
            print(f"Result: {result}")
    except FileNotFoundError:
        print(f"File not found: {filename}")
        result = False
    except Exception as e:
        print(f"Error reading {filename}: {e}")
        result = False

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Memory: {peak_memory_mb:.2f} MB")
    print()

def main():
    dataset_files = [
        r"C:\Users\User\Desktop\aisf\datasets\11\1000.txt",
        r"C:\Users\User\Desktop\aisf\datasets\11\2000.txt",
        r"C:\Users\User\Desktop\aisf\datasets\11\3000.txt",
        r"C:\Users\User\Desktop\aisf\datasets\11\4000.txt"
    ]

    for filename in dataset_files:
        process_file(filename)

if __name__ == "__main__":
    main()