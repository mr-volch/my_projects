a = input()
humans = {}
decks = {}

while a:
    a = a.split(' / ')
    if a[0].isdigit():
        decks.setdefault(a[0], set({}))
        decks[a[0]].add(a[1])
    else:
        humans.setdefault(a[0], set({}))
        humans[a[0]].add(a[1])
    a = input()

human_list = list(humans.keys())
for i in human_list:
    cur_set = set({})
    for j in humans[i]:
        cur_set = cur_set.union(decks[j])
    humans[i] = len(cur_set)

humans = dict(sorted(humans.items(), key=lambda item: -item[1]))
human_list = list(humans.keys())
i = 0
outro = []
while i < len(human_list) and humans[human_list[i]] == humans[human_list[0]]:
    outro.append(human_list[i])
    i += 1

outro.sort()
print('\n'.join(outro))
