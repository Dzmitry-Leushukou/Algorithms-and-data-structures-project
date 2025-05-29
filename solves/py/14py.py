import time
import psutil
import random

def find_max_square(matrix):
    if not matrix or not matrix[0]:
        return 0

    n, m = len(matrix), len(matrix[0])
    dp = [[0] * m for _ in range(n)]

    for j in range(m):
        dp[0][j] = matrix[0][j]
    for i in range(n):
        dp[i][0] = matrix[i][0]

    for i in range(1, n):
        for j in range(1, m):
            if matrix[i][j] == 1:
                dp[i][j] = min(dp[i][j-1], dp[i-1][j-1], dp[i-1][j]) + 1

    max_size = 0
    for i in range(n):
        for j in range(m):
            max_size = max(max_size, dp[i][j])

    return max_size

def process_file(filename):
    start_time = time.perf_counter()
    process = psutil.Process()

    try:
        # Generate random matrix
        random.seed(time.time())
        matrix = [[1 if random.random() < 0.7 else 0 for _ in range(10000)] for _ in range(1000)]

        # Write matrix to file
        with open(filename, 'w') as f:
            for row in matrix:
                f.write(" ".join(map(str, row)) + "\n")

        # Solve and append result
        result = find_max_square(matrix)
        with open(filename, 'a') as f:
            f.write(str(result) + "\n")

        print(f"Processing file: {filename}")
    except Exception as e:
        print(f"Error processing {filename}: {e}")

    end_time = time.perf_counter()
    memory_info = process.memory_info()
    peak_memory_mb = memory_info.rss / (1024 * 1024)

    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Memory: {peak_memory_mb:.2f} MB")
    print()

def main():
    dataset_files = [
        r"C:\Users\User\Desktop\aisf\datasets\14\1.txt",
        r"C:\Users\User\Desktop\aisf\datasets\14\2.txt",
        r"C:\Users\User\Desktop\aisf\datasets\14\3.txt"
    ]

    for filename in dataset_files:
        process_file(filename)

if __name__ == "__main__":
    main()