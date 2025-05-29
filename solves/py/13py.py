import time
import random
import psutil
import sys

class Set:
    def __init__(self, fanta, banana, cake, cost):
        self.fanta = fanta
        self.banana = banana
        self.cake = cake
        self.cost = cost

def solve(n, mf, mb, mc, m, sets):
    # Initialize DP table
    dp = [[[ [float('inf'), []] for _ in range(mc+1)] for _ in range(mb+1)] for _ in range(mf+1)]
    dp[0][0][0] = [0, []]
    
    for i in range(m):
        f = sets[i].fanta
        b = sets[i].banana
        c = sets[i].cake
        cost = sets[i].cost
        
        for ff in range(mf, -1, -1):
            for bb in range(mb, -1, -1):
                for cc in range(mc, -1, -1):
                    if dp[ff][bb][cc][0] != float('inf'):
                        newf = min(mf, ff + f)
                        newb = min(mb, bb + b)
                        newc = min(mc, cc + c)
                        newcost = dp[ff][bb][cc][0] + cost
                        
                        if newcost < dp[newf][newb][newc][0]:
                            dp[newf][newb][newc][0] = newcost
                            dp[newf][newb][newc][1] = dp[ff][bb][cc][1] + [i + 1]
    
    if dp[mf][mb][mc][0] == float('inf'):
        return
    
    mincost = dp[mf][mb][mc][0]
    mincontribution = (mincost + n - 1) // n

def main():
    filename = "./datasets/13/1.txt"
    random.seed(time.time())
    sets = []
    n = 1
    mf = 10
    mb = 10
    mc = 10
    m = 10
    
    with open(filename, 'w') as fout:
        fout.write(f"{n} {mf} {mb} {mc} {m}\n")
        for i in range(m):
            fanta = random.randint(0, 5)
            banana = random.randint(0, 5)
            cake = random.randint(0, 5)
            cost = random.randint(1, 100)
            fout.write(f"{fanta} {banana} {cake} {cost}\n")
            sets.append(Set(fanta, banana, cake, cost))
    
    process = psutil.Process()
    start_time = time.time()
    mem_before = process.memory_info().rss
    
    solve(n, mf, mb, mc, m, sets)
    
    end_time = time.time()
    mem_after = process.memory_info().rss
    peak_mem_bytes = process.memory_info().peak_wset if hasattr(process.memory_info(), 'peak_wset') else mem_after
    
    print(f"Time: {end_time - start_time:.6f} s")
    print(f"Mem: {peak_mem_bytes / (1024 * 1024):.2f} MB")

if __name__ == "__main__":
    main()