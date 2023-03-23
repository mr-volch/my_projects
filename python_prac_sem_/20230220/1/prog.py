import cowsay
from typing import *


#Класс монстров
class Monster:
    #инициализируем нового монстра: вносим приветствие и имя
    def __init__(self, hello : str):
        self.hello = hello


    #функция, реализующая битву с монстром. Ничего не возвращает
    def battle(self):
        #Выводим монстра с приветствием
        print(cowsay.cowsay(self.hello))
        return None



#Класс локаций(карт)
class Location:
    #иницализируем новую локацию: задаем размеры и заводим пустой словарь монстров
    def __init__(self, size_x : int = 10, size_y : int = 10):
        self.size_x = size_x
        self.size_y = size_y
        #формат словаря: ключи - кортежи с координатами монстров (x,y),
        #значения - объекты класса Monster
        self.monsters = {}


    #служебный метод, возвращающий кортеж с размерами локации
    def get_size(self):
        return (self.size_x, self.size_y)


    #метод, реализующий добавление монстра на клетку с координатами x,y и
    #приветствием hello. Возвращает объект - монстра.
    def add_monster(self, x : int, y : int, hello : str) -> Monster:
        #проверяем наличие монстра по указанным координатам
        flag = (x,y) in self.monsters

        #Кладем в словарь монстров монстра с данным hello по ключу (x,y)
        self.monsters[(x,y)] = Monster(hello)

        #Сообщаем игроку о произошедшем
        print(f"Added monster to ({x}, {y}) saying {hello}")
        if flag:
            print("Replaced the old monster")

        #Возвращаем нового монстра
        return self.monsters[(x,y)]


    #Метод, реализующий происшествие при встрече игрока с монстром
    #Возвращает булевое значение: был ли бой?
    def encounter(self, x : int, y : int) -> bool:
        #Если по указанным координатам есть монстр (т.е. кортеж с его координатами есть
        #в ключах словаря с монстрами), то запускаем битву с ним
        if (x,y) in self.monsters:
            self.monsters[(x,y)].battle()
            return True
        return False



#Класс игроков
class Player:
    #Инициализируем игрока: задаем локацию(Объект класса Location) и координаты точки спавна
    def __init__(self, x : int = 0, y : int = 0, location : Location = Location()):
        self.x = x
        self.y = y
        self.location = location

    #Метод для передвижения игрока. Возвращает кортеж с итоговыми координатами.
    def move(self, cmd : str) -> Tuple[int, int]:
        #Меняем координаты в зависимости от команды с учетом того, что:
        # 1) нумерация клеток увеличивается сверху вниз, слева направо
        # 2) если игрок переходит границу, он появляется с обратной стороны поля
        #    (реализовано при помощи операции % от соответсвющего габарита поля)
        location_sizes = self.location.get_size()
        match cmd:
            case 'up':
                self.y = (self.y - 1) % location_sizes[1]
            case 'down':
                self.y = (self.y + 1) % location_sizes[1]
            case 'left':
                self.x = (self.x - 1) % location_sizes[0]
            case 'right':
                self.x = (self.x + 1) % location_sizes[0]

        #Сообщаем игроку о произошедшем и запускаем обработчик происшествия для текущей локации
        print(f"Moved to ({self.x}, {self.y})")   
        self.location.encounter(self.x, self.y)

        return (self.x, self.y)



#Функция для проверки корректности аргументов given_args
#(они должны соответствовать real_args)
def check_args(given_args, real_args):
    return False



#функция, делающая кортеж с параметрами для запускаемых команд
#аргументы: 1) словарь доступных команд available_cmds в формате {<команда>:<класс>.{<полe>.}<метод>}
#           2) сама по себе команда (а именно список слов ее составляющих, полученый из .split())
def make_args(player : Player, available_cmds : Dict[str, callable], cmd : List[str]):
    #Текущий метод класса
    try:
        cur_cmd = available_cmds[cmd[0]]
    except:
        return 1

    cur_annotations = cur_cmd[1].__annotations__

    match cur_cmd[0]:
        case 'Player':
            args = [player, cmd[0]]
        case 'Location':
            args = [player.location]

    n = 1
    for i in cur_annotations:
        if n == len(cmd):
            break
        try:
             args.append(cur_annotations[i](cmd[n]))
             n += 1
        except:
            return 2

    return tuple(args)
        

#функция, запускающая основной геймплей
def gameplay():
    #Инициализируем словарь доступных команд и игрока в виде объекта класса Player
    #суть содержимого словаря - отображение множества пользовательских команд во множество методов классов
    available_cmds = {'up': ('Player', Player.move), 'down': ('Player', Player.move), 'left': ('Player', Player.move),
                      'right': ('Player', Player.move), 'addmon': ('Location', Location.add_monster)}
    player = Player()

    #бесконечно читаем команды
    while cmd := input():
        cmd = cmd.split()

        cur_args = make_args(player, available_cmds, cmd)
        match cur_args:
            case 1:
                print("invalid command")
            case 2:
                print("invalid arguments")
            case _:
                available_cmds[cmd[0]][1].__call__(*cur_args)



#Запуск игры
gameplay()
