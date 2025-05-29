import time
import psutil
import random

class BigInt:
    def __init__(self, num=0):
        self.digits = []
        if num == 0:
            self.digits.append(0)
        else:
            while num:
                self.digits.append(num % 10)
                num //= 10

    def remove_leading_zeros(self):
        while len(self.digits) > 1 and self.digits[-1] == 0:
            self.digits.pop()

    def to_string(self):
        if not self.digits:
            return "0"
        return "".join(str(d) for d in reversed(self.digits))

    def __add__(self, other):
        result = BigInt()
        carry = 0
        i = 0
        while i < len(self.digits) or i < len(other.digits) or carry:
            a = self.digits[i] if i < len(self.digits) else 0
            b = other.digits[i] if i < len(other.digits) else 0
            total = a + b + carry
            carry = total // 10
            result.digits.append(total % 10)
            i += 1
        return result

    def __sub__(self, other):
        result = BigInt()
        borrow = 0
        i = 0
        while i < len(self.digits):
            a = self.digits[i]
            b = other.digits[i] if i < len(other.digits) else 0
            sub = a - b - borrow
            if sub < 0:
                sub += 10
                borrow = 1
            else:
                borrow = 0
            result.digits.append(sub)
            i += 1
        result.remove_leading_zeros()
        return result

    def __mul__(self, other):
        n, m = len(self.digits), len(other.digits)
        temp = [0] * (n + m)
        for i in range(n):
            for j in range(m):
                temp[i + j] += self.digits[i] * other.digits[j]
        carry = 0
        for i in range(n + m):
            total = temp[i] + carry
            carry = total // 10
            temp[i] = total % 10
        result = BigInt()
        result.digits = temp
        result.remove_leading_zeros()
        return result

    def divide(self, divisor):
        temp = []
        remainder = 0
        for d in reversed(self.digits):
            current = remainder * 10 + d
            temp.append(current // divisor)
            remainder = current % divisor
        temp.reverse()
        while len(temp) > 1 and temp[0] == 0:
            temp.pop(0)
        result = BigInt()
        result.digits = temp
        return result

def process_file(filename, n=10, k=13):
    start_time = time.perf_counter()
    process = psutil.Process()
    
    try:
        if n == 0:
            with open(filename, 'w') as f:
                f.write("0\n")
            return

        pow_k = [BigInt(0) for _ in range(51)]
        pow_k[0] = BigInt(1)
        for i in range(1, 51):
            pow_k[i] = pow_k[i-1] * BigInt(k)

        T = [BigInt(0)] * (n + 1)

        for s in range(n-1, -1, -1):
            F_val = BigInt(0)
            if s == 0:
                if n >= 1:
                    if k == 1:
                        F_val = BigInt(n)
                    else:
                        numerator = (pow_k[n] - BigInt(1)) * BigInt(k)
                        F_val = numerator.divide(k - 1)
            else:
                if n - s - 1 >= 1:
                    if k == 1:
                        F_val = BigInt(n - s - 1)
                    else:
                        numerator = (pow_k[n - s - 1] - BigInt(1)) * BigInt(k)
                        F_val = numerator.divide(k - 1)

            sum_terms = BigInt(0)
            if s == 0:
                for d in range(1, n - 1):
                    t = d
                    term = pow_k[d] * T[t]
                    sum_terms = sum_terms + term
            else:
                if n - s - 3 >= 0:
                    for d in range(1, n - s - 2):
                        t = s + 1 + d
                        if t > n:
                            continue
                        term = pow_k[d] * T[t]
                        sum_terms = sum_terms + term

            T[s] = F_val + sum_terms

        with open(filename, 'w') as f:
            f.write(T[0].to_string() + "\n")
        
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
        r"C:\Users\User\Desktop\aisf\datasets\12\max.txt"
    ]

    for filename in dataset_files:
        process_file(filename, n=10, k=13)

if __name__ == "__main__":
    main()