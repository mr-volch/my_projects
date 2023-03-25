diego_amount = int(input())

diego_cards = sorted(list(set([int(i) for i in (input()).split()])))
diego_amount = len(diego_cards)

collectioners_amount = int(input())

dict_tmp = {}

a = input().split()

for i in range(len(a)):
    dict_tmp[i] = int(a[i])

collectioners_cards = dict(sorted(dict_tmp.items(), key = lambda item: item[1]))

left = 0

for j in collectioners_cards:
    cur_card = collectioners_cards[j]
    right = diego_amount-1
    cur = (right-left)//2

    while left <= right:
        if diego_cards[cur] == cur_card:
            while diego_cards[cur] == cur_card and cur >= 0:
                cur -= 1
            break
        elif diego_cards[cur] < cur_card:
            left = cur + 1
        else:
            right = cur - 1
            
        cur = left + (right-left)//2
    else:
        collectioners_cards[j] = left
        continue
    
    collectioners_cards[j] = cur+1

for i in range(collectioners_amount):
    print(collectioners_cards[i])
            
