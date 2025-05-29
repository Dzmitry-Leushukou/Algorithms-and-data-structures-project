import time
import psutil

def main():
    start_time = time.perf_counter()
    process = psutil.Process()
    
    sum_result = 0
    for i in range(1, 1001):
        sum_result += i

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Mem: {peak_memory_mb:.2f} MB")

if __name__ == "__main__":
    main()