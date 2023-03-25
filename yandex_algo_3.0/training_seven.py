def make_int_list(input_list):
    for i in range(len(input_list)):
        if input_list[i][0] == '0':
            input_list[i] = int(input_list[i][1])
        else:
            input_list[i] = int(input_list[i])
    return input_list

def make_seconds(input_list):
    return input_list[0]*3600 + input_list[1]*60 + input_list[2]

first_time = input().split(':')
first_time = make_int_list(first_time)
first_seconds = make_seconds(first_time)

real_time = input().split(':')
real_time = make_int_list(real_time)
real_seconds = make_seconds(real_time)

second_time = input().split(':')
second_time = make_int_list(second_time)
second_seconds = make_seconds(second_time)

diff_seconds = None

if second_seconds < first_seconds:
    if (second_seconds + (86400 - first_seconds)) % 2 == 1:
        diff_seconds = ((second_seconds + (86400 - first_seconds))//2 + 1)
    else:
        diff_seconds = (second_seconds + (86400 - first_seconds))//2

else:
    if (second_seconds - first_seconds)%2 == 1:
        diff_seconds = ((second_seconds - first_seconds)//2 + 1)
    else:
        diff_seconds = (second_seconds - first_seconds)//2

res_time = (real_seconds + diff_seconds) % 86400

hours = str(res_time // 3600)
minutes = str((res_time % 3600) // 60)
seconds = str(res_time % 60)

if len(hours) == 1:
    hours = '0' + hours
if len(minutes) == 1:
    minutes = '0' + minutes
if len(seconds) == 1:
    seconds = '0' + seconds

print(hours+':'+minutes+':'+seconds)
