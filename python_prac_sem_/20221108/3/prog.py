class Undead(Exception): pass

class Skeleton(Undead): pass

class Zombie(Undead): pass

class Ghoul(Undead): pass

def necro(a):
    if a%3 == 0:
        raise Skeleton
    elif a%3 == 1:
        raise Zombie
    else:
        raise Ghoul

a = eval(input())
be = a[0]
en = a[1]

for i in range(be,en):
    try:
        necro(i)
    except Skeleton:
        print("Skeleton")
    except Zombie:
        print("Zombie")
    except Ghoul:
        print("Generic Undead")
        
