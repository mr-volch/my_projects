def No_2Zero(N,K):
    if N == 1:
        return K-1
    elif N == 2:
        return (K-1)*K
    else:
        return (K-1)*(No_2Zero(N-1,K) + No_2Zero(N-2,K))

##print(No_2Zero(6, 3))
