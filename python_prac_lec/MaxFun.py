def maxfun(S, F1, *Fi):
    max_f = F1
    max_s = 0

    for x in S:
        max_s += F1(x)
    
    for f in Fi:
        cur = 0
        for x in S:
            cur += f(x)
        if cur >= max_s:
            max_s = cur
            max_f = f
             
    return max_f
