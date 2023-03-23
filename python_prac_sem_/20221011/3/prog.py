from math import *

intro = ''
x = 0
y = 0
water = 0
while a := input():
    if a[1] == '~':
        water += 1
    x = len(a)
    y += 1
    intro += a + '\n'

total_water = water * (x-2)
total_gas = (y - water - 2) * (x-2)

new_water = total_water/(y-2)
if not(isclose(trunc(new_water), new_water)):
    new_water = trunc(new_water) + 1
else:
    new_water = trunc(new_water)

outro = '#' * y
for i in range(x-2-new_water):
    outro += '\n' + '#' + '.' * (y-2) + '#'

for i in range(new_water):
    outro += '\n' + '#' + '~' * (y-2) + '#'
outro += '\n' + '#' * y

print(outro)
if (x-2-new_water) < new_water: 
    number_of_dots = round((x-2-new_water)/new_water*20)
    first_diag_str = '.' * number_of_dots + ' ' * (21 - number_of_dots \
    + len(str(new_water*(y-2))) - len(str((x-2-new_water)*(y-2)))) \
    + str((x-2-new_water)*(y-2)) + '/' + str((x-2)*(y-2))

    second_diag_str = '~' * 20 + ' '+ str(new_water*(y-2)) + '/' + str((x-2)*(y-2))

    print(first_diag_str)
    print(second_diag_str)

else:
    number_of_waves = round(new_water/(x-2-new_water)*20)
    first_diag_str = '.' * 20 + ' ' + str((x-2-new_water)*(y-2)) + '/' + str((x-2)*(y-2))

    second_diag_str = '~' * number_of_waves + ' ' * (21 - number_of_waves \
    + len(str((x-2-new_water)*(y-2))) - len(str(new_water*(y-2)))) \
    + str(new_water*(y-2)) + '/' + str((x-2)*(y-2))

    print(first_diag_str)
    print(second_diag_str)
