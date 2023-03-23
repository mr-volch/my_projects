from fractions import Fraction

intro = input()

char_list = ['+', '-', '/', '*', '%', '(', ')', ]
for i in char_list:
    intro = intro.replace(i, ' '+i+' ')
    
intro = intro.split()

for i in range(len(intro)):
    if intro[i][0].isdigit() or intro[i][0] == '.':
        cur = Fraction(intro[i])
        intro[i] = 'Fraction' + '(' + str(cur.numerator) + ',' + ' ' + str(cur.denominator) + ')' 

intro = ''.join(intro)

print(eval(intro))        

