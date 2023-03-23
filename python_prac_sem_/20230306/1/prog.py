import cowsay
from typing import *
import readline
import shlex
import cmd
from io import StringIO

jgsbat = cowsay.read_dot_cow(StringIO("""
$the_cow = <<EOC;
         $thoughts
          $thoughts
    ,_                    _,
    ) '-._  ,_    _,  _.-' (
    )  _.-'.|\\\\\--//|.'-._  (
     )'   .'\/o\/o\/'.   `(
      ) .' . \====/ . '. (
       )  / <<    >> \  (
        '-._/``  ``\_.-'
  jgs     __\\\\\\\'--'//__
         (((""`  `"")))
EOC
"""))

#Класс монстров
class Monster:
    #инициализируем нового монстра: вносим приветствие и имя
    def __init__(self, hello : str, name : str, hp : int):
        self.hello = hello
        self.name  = name
        self.hp = hp

    def get_name(self) -> str:
        return self.name

    #функция, реализующая битву с монстром. Ничего не возвращает
    def greetings(self):
        #Выводим монстра с приветствием
        if self.name == 'jgsbat':
            print(cowsay.cowsay(self.hello, cowfile = jgsbat))
        else:
            print(cowsay.cowsay(self.hello, cow = self.name))

        return None

    def battle(self, damage: int) -> tuple[int]:
        prev_hp = self.hp
        self.hp -= damage
        if self.hp <= 0:
            return (0, prev_hp, self.name)
        else:
            return (self.hp, damage, self.name)


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


    #метод, реализующий добавление монстра name на клетку с координатами x,y и
    #приветствием hello. Возвращает объект - монстра.
    def add_monster(self, name : str, hello : str, hp : int, x : int, y : int) -> Monster:
        #Проверяем корректность имени данного монстра. Если некорр. - возвращаем None
        if name not in cowsay.list_cows() and name != 'jgsbat' :
            print("Cannot add unknown monster")
            return None
        
        #проверяем наличие монстра по указанным координатам
        flag = (x,y) in self.monsters

        #Кладем в словарь монстров монстра с данным hello и name по ключу (x,y)
        self.monsters[(x,y)] = Monster(hello, name, hp)
        #Сообщаем игроку о произошедшем
        print(f"Added monster {name} to ({x}, {y}) saying {hello}")
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
            self.monsters[(x,y)].greetings()
            return True
        return False

    def encounter_attack(self, x: int, y: int, name: str, damage : int) -> Monster:
        if (x,y) not in self.monsters:
            print("No monster here")
            
        elif self.monsters[(x,y)].get_name() != name:
                print(f"No monster {name} here")

        else:
            res = self.monsters[(x,y)].battle(damage)
            print(f"Attacked {res[2]}, damage {res[1]} hp")
            
            if res[0] == 0:
                print(f"{res[2]} died")
                self.monsters.pop((x,y))
            else:
                print(f"{res[2]} now has {res[0]} hp")

    def get_monsters(self) -> list[str]:
        res = []
        for coord_set in self.monsters:
            cur_monster = self.monsters[coord_set]
            res.append(cur_monster.get_name())
        return res
                

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

    def init_monster(self, name : str, hello : str, hp : int, x : int, y : int) -> Monster:
        return self.location.add_monster(name, hello, hp, x, y)

    def attack(self, name:str, weapon : str):
        weapons = {'sword': 10, 'spear': 15, 'axe': 20}
        if weapon == None:
            weapon = 'sword'
        
        if weapon not in weapons:
            print("Unkown weapon")
        else:
            damage = weapons[weapon]
            self.location.encounter_attack(self.x, self.y, name, damage)

    def available_monsters(self) -> list[str]:
        return self.location.get_monsters()



###функция, делающая кортеж с параметрами для запускаемых команд
###аргументы: 1) Переменная класс player для которой делаем команду
###           2) словарь доступных команд available_cmds в формате {<команда>:<класс>.{<полe>.}<метод>}
###           3) сама по себе команда (а именно список слов ее составляющих, полученый из .split())
###возвращаем: 1 - плохая команда, 2 - плохие аргументы, кортеж - успех. кортеж для вызова
def make_args(player : Player, available_cmds : Dict[str, callable], cmd : List[str]):
    #Вычленяем нужный метод класса (кортеж вида : (<Название класса(str)>, <метод класса>))
    try:
        cur_cmd = available_cmds[cmd[0]]
    except:
        return 1

    #Получаем аннотации для нашего. метода
    cur_annotations = cur_cmd[1].__annotations__

    #В зависимости от того, для какого класса делаем, заполняем начальные позиции аргументов
    match cur_cmd[0]:
        case "":
            #Здесь добавляем сам объект и команду (напр. 'up')
            args = [player, cmd[0]]
        case cmd_args:
            #здесь добавляем локацию игрока
            args_len = 1
            for i in cmd_args:
                args_len += len(cmd_args[i])
            
            args = [None for i in range(args_len)]
            args[0] = player

            #Пробегаемся по всем поданным аргументам
            n = 0

            if 1:
                while n < len(cmd):
                    if cmd[n] in cmd_args.keys():
                        args_amount = len(cmd_args[cmd[n]])
                        cur_arg = cmd_args[cmd[n]]
                        n += 1
                    else:
                        raise AttributeError

                    for j in range(args_amount):
                        args[cur_arg[j]] = cmd[n]
                        n += 1
                arg_ind = 1
                for i in cur_annotations:
                    if arg_ind == args_len:
                        break
                    if args[arg_ind]:
                        args[arg_ind] = (cur_annotations[i](args[arg_ind]))
                        arg_ind += 1
            else:
                return 2

    #возвращаем кортеж с аргументами
    return tuple(args)
        

class game_shell(cmd.Cmd):
    available_cmds = {'up': ('', Player.move), 'down': ('', Player.move), 'left': ('', Player.move),
                      'right': ('', Player.move), 'attack' : ({'attack' : (1,), 'with':(2,)}, Player.attack), 'addmon': ({'addmon' : (1,), 'hello' : (2,), 'hp' : (3,), 'coords' : (4,5)}, Player.init_monster)}
    player = Player()
    cur_monster_pos = 0
    cur_weapon_pos = 0

    def caller(cmd, cur_args):
        match cur_args:
            case 1:
                print("invalid command")
            case 2:
                print("invalid arguments")
            case _:
                #В случае успеха вызываем соотв. метод с распакованными аргументами
                game_shell.available_cmds[cmd[0]][1].__call__(*cur_args)

    def do_up(self, args):
        cur_args = shlex.split(args)
        cur_args.insert(0, 'up')
        fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
        game_shell.caller(cur_args, fin_args)

    def do_down(self, args):
        cur_args = shlex.split(args)
        cur_args.insert(0, 'down')
        fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
        game_shell.caller(cur_args, fin_args)
        
    def do_right(self, args):
        cur_args = shlex.split(args)
        cur_args.insert(0, 'right')
        fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
        game_shell.caller(cur_args, fin_args)

    def do_left(self, args):
        cur_args = shlex.split(args)
        cur_args.insert(0, 'left')
        fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
        game_shell.caller(cur_args, fin_args)

    def do_attack(self, args):
        game_shell.cur_weapon_pos = 0
        game_shell.cur_monster_pos = 0
        cur_args = shlex.split(args)
        
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('addmon')
        else:
            cur_args.insert(0, 'attack')
            fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
            game_shell.caller(cur_args, fin_args)

    def help_attack(self, args):
        print("""format: attack <monster_name >with <weapon_name>
    attack monster with name <monster_name> with weapon <weapon_name>, if rhere is a monster on your current position""")

    def complete_attack(self, text, line, begidx, endidx):
        target_attack = game_shell.player.available_monsters()
        target_with = ["sword", "spear", "axe"]

        line = line.split()
        if line[-1] in target_with or line[-1] == 'with':
                completions = [target_with[game_shell.cur_weapon_pos]]
                game_shell.cur_weapon_pos = ((game_shell.cur_weapon_pos + 1) % len(target_with))
        else:
            completions = [target_attack[game_shell.cur_monster_pos]]
            game_shell.cur_monster_pos = ((game_shell.cur_monster_pos + 1) % len(target_attack))
            
        return completions

    def do_addmon(self, args):
        cur_args = shlex.split(args)

        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('addmon')
        else:
            cur_args.insert(0, 'addmon')
            fin_args = make_args(game_shell.player, game_shell.available_cmds, cur_args)
            game_shell.caller(cur_args, fin_args)

    def help_addmon(self):
        print("""format: addmon <monster_name> hello <hello_string> hp <hitpoints> coords <x> <y>
    adds a new monster to player`s location
    param <monster_name>: one of available ascii arts for python-cowsay module
    param hello: hello string
    param hp: monster`s hit points
    param coords: coordinates of exect place on location for monster""")

    def complete_addmon(self, text, line, begidx, endidx):
        target_addmon = ["-h", "--help", "<monster name>"]
        target_cows= ["hello", "hp", "coords"]
        target_coords = ["0 0", "1 1", "2 2"]
        target_hp = ["40", "50", "60"]
        target_hello = ["hello", "'you will be doomed!'", "'there is no way you beat me!'"]

        line = line.split()
        match line[-1]:
            case 'addmon':
                completions = target_addmon
            case 'coords':
                completions = target_hello
            case 'hp':
                completions = target_hp
            case 'hello':
                completions = target_hello
            case _:
                completions = target_cows

        return completions


#Запуск игры
if __name__ == '__main__':
    print("<<< Welcome to Python-MUD 0.1 >>>")
    game_shell().cmdloop()
