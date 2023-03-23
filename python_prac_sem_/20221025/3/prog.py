import itertools as it

print(*["".join(z) for z in sorted(list(filter(lambda x: "".join(x).count("TOR") >= 2 ,(it.product('TOR', repeat = int(input()))))), key = lambda y: "".join(y))], sep = ', ')
