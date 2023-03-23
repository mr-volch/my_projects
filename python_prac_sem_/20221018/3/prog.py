import string

len_search = int(input())
popularity_dict = {}

while s := input():
    s = "".join((lambda ch: ch if ch.isalpha() else ' ')(ch) for ch in s)
    s = s.lower().split()
    s = [w for w in s if len(w) == len_search]

    for i in s:
        popularity_dict.setdefault(i, 0)
        popularity_dict[i] += 1

popularity_dict = dict(sorted(popularity_dict.items(), key = lambda item: item[1], reverse = True))
    
prev = -1
for s in popularity_dict:
    if popularity_dict[s] == prev or prev == -1:
        print(s, end = ' ')
        prev = popularity_dict[s]
    else:
        print()
        break
