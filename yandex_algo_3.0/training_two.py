##all_letters = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']
##
n = int(input())
s = input()

all_letters = set(s)

best_beauty = n

if len(s) <= n:
    print(len(s))
else:
    for letter in all_letters:
        
        letter_amount = 0
        cur_beauty = n
        left = 0
        right = 0

        while right < len(s):
            tmp = right - left
            
            if tmp < letter_amount + n:
                while right < len(s) and s[right] == letter:
                    letter_amount += 1
                    right += 1
                cur_beauty = max(right-left, cur_beauty)
                right += 1
                
            else:
                while right < len(s) and s[right] == letter:
                    letter_amount += 1
                    right += 1
                cur_beauty = max(right-left, cur_beauty)
                while s[left] == letter and left <= right:
                    left += 1
                    letter_amount -= 1
                left += 1

        
        best_beauty = max(best_beauty, cur_beauty)

    print(best_beauty)
