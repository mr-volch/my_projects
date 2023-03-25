def count_hops(N,K):
    places = [0 for i in range(N)]
    places[0] = 1
    for i in range(1, N):
        places[i] = sum(places[max(0,i-K):i])
    return places[-1]


intro = input().split()
N,K = (int(intro[0]), int(intro[1]))
print(count_hops(N,K))
