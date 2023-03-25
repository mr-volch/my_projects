N = int(input())
K = int(input())
row = int(input())
place = int(input())

if (row-1)*2 + place <= K and N - (row-1)*2 - place < K:
    print(-1)
else:
    right = (row-1)*2 + place - K
    left = (row-1)*2 + place + K
    if left <= N:
        print((left+1)//2, (1 if left%2 else 2))
    else:
        print((right+1)//2, (1 if right%2 else 2))
