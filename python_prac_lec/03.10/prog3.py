a = input()

if a:
    a = eval(a)

pipeline = []

if isinstance(a, tuple):
    for i in a:
        if isinstance(i, tuple):
            for j in i:
                pipeline.append(j)
        else:
            if len(pipeline) >= i:
                cur = []
                for j in range(i):
                    cur.append(pipeline[0])
                    pipeline.pop(0)
                cur = tuple(cur)
                print(cur)
            else:
                break
                
