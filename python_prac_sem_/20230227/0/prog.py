import shlex
import readline

###0
##print(shlex.split("qwe 'ret     tyui'"))
##print(shlex.join(["qwre", "wqre qwre"]))


###1
##while (string_in := input()) != 'quit':
##    print(shlex.join(shlex.split(string_in)))


#2
FIO = input()
birth_plase = input()
cur = shlex.join(["register", FIO, birth_plase])
print(cur)

print(shlex.split(cur))

res2 = input("simplier result")
print(shlex.split(res2))
