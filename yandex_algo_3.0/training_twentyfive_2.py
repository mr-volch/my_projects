def count_length(N, coords):
    if N == 2:
        return coords[1] - coords[0]

    else:
        best_ways = [0 for i in range(N)]
        best_ways[1] = coords[1] - coords[0]
        best_ways[2] = coords[2] - coords[0]
        for i in range(3, N):
            best_ways[i] = coords[i] - coords[i-1] + min(best_ways[i-1], best_ways[i-2])
        
        return best_ways[-1]


N = int(input())
coords = sorted([int(i) for i in input().split()])

print(count_length(N, coords))
    
