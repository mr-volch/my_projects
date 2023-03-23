from prettytable import PrettyTable

# Заполняем шапку таблицы и формируем ее наполнение
header = ["Кол-во итераций \ Кол-во нитей/процессов"]
main_data = {}

#Читаем данные из файла со статистикой. Формат записи в файле:
#<кол-во нитей/процессов> <кол-во итераций> <результат вычисления> <время выполнения (в мс)>
with open(input("Введите файл, для которого хотиите просмотреть статистику: "),'r') as f:
    while c:=f.readline():
        tmp = c.split()
        if int(tmp[0]) not in header:
            header.append(int(tmp[0]))
        main_data.setdefault(int(tmp[1]), {})
        main_data[int(tmp[1])].setdefault(int(tmp[0]), [0, 0])
        main_data[int(tmp[1])][int(tmp[0])][0] += float(tmp[3])
        main_data[int(tmp[1])][int(tmp[0])][1] += 1

#В зависимости от ответа пользователя будем выводить время выполнения(в мс)
#или ускорение по сравнению с последовательной программой
answ = int(input("""Если Вы хотите увидеть время выполнения программы, в зависимости от кол-ва используемых ресурсов, введите 1
Если Вы хотите увидеть ускорение выполнения программой по отношению к последовательной версии, введите 2\n"""))

#инициализируем таблицу
header[1:]=sorted(header[1:])
myTable = PrettyTable(header)

if answ not in (1,2):
    print("Запустите заново и введите 1 или 2")

#Если ответ - 1, то выводим времена выполнения
elif answ == 1:
    #Добавляем строки в таблицу
    eps = int(input("введите кол-во знаков после запятой в выводе: "))
    sorted_rows = sorted(main_data.keys())
    num = 0
    for row in sorted_rows:
        cur_row = [row]
        sorted_keys = sorted(main_data[row].keys())
        for key in sorted_keys:
            cur_row.append(format(main_data[row][key][0]/main_data[row][key][1], f'.{eps}f'))
        myTable.add_row(cur_row)
        num += 1
    print('\n', myTable, sep = '')
    print('\nВ этой таблице отражено время выполнения программы при разном числе итераций в зависимости от используемого числа нитей/процессов')

else:
    eps = int(input("введите кол-во знаков после запятой в выводе: "))
    base_times = {}
    amount = 0
    #читаем информацию о выполнения последовательной программы
    with open('base','r') as f:
        while c:=f.readline():
            tmp = c.split()
            base_times.setdefault(tmp[0], [0,0])
            base_times[tmp[0]][0] += float(tmp[1])
            base_times[tmp[0]][1] += 1
    times = {}
    sorted_times = sorted(base_times.keys())
    for time in sorted_times:
        times[int(time)] = (base_times[time][0]/base_times[time][1])

    #Добавляем строки в таблицу
    sorted_rows = sorted(main_data.keys())
    num = 0
    for row in sorted_rows:
        cur_row = [row]
        sorted_keys = sorted(main_data[row].keys())
        for key in sorted_keys:
            cur_row.append(format(times[row]/(main_data[row][key][0]/main_data[row][key][1]), f'.{eps}f'))
        myTable.add_row(cur_row)
        num += 1
    print('\n', myTable, sep = '')
    print('\nВ этой таблице отражено ускорение выполнения программы по сравнению с последовательной программой в зависмости от используемого числа нитей/процессов')
