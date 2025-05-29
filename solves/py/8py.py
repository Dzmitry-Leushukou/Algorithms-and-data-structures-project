import time
import psutil
import struct

def main():
    start_time = time.perf_counter()
    process = psutil.Process()
    
    filename = "./datasets/8/1000_2.bin"
    count = 0
    try:
        with open(filename, 'rb') as f:
            while True:
                data = f.read(4)
                if not data:
                    break
                if len(data) == 4:
                    num = struct.unpack('i', data)[0]
                    if num > 0:
                        count += 1
    except FileNotFoundError:
        print(f"File not found: {filename}")
    except Exception as e:
        print(f"Error reading {filename}: {e}")

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Mem: {peak_memory_mb:.2f} MB")

if __name__ == "__main__":
    main()