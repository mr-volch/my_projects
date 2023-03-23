import cowsay
import asyncio
import readline
import shlex
import cmd
from io import StringIO
from typing import *


# кастомный ascii арт
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


class Monster:
    """Класс монстров"""

    def __init__(self, hello : str, name : str, hp : int):
        """инициализируем нового монстра: вносим приветствие и имя"""
        
        self.hello = hello
        self.name  = name
        self.hp = hp


    def get_name(self) -> str:
        """getter имени монстра"""
        return self.name


    def greetings(self):
        """метод, реализующая встречу с монстром.
        Возвращает приветствие, оформленное модулем cowsay"""

        if self.name == 'jgsbat':
            return cowsay.cowsay(self.hello, cowfile = jgsbat)
        
        return cowsay.cowsay(self.hello, cow = self.name)


    def battle(self, damage: int) -> tuple[int]:
        """метод, реализующий битву с монстром.
        Примает: наносимый урон
        Возвращает: кортеж вида: (остаток жизней, нанесенный урон, имя)"""
        
        prev_hp = self.hp
        self.hp -= damage
        if self.hp <= 0:
            return (0, prev_hp, self.name)
        else:
            return (self.hp, damage, self.name)


class Location:
    """Класс локаций(карт)"""
    
    def __init__(self, size_x : int = 10, size_y : int = 10):
        """иницализируем новую локацию: задаем размеры и заводим пустой словарь монстров"""
        
        self.size_x = size_x
        self.size_y = size_y
        # формат словаря: ключи - кортежи с координатами монстров (x,y),
        #                 значения - объекты класса Monster
        self.monsters = {}


    def get_size(self):
        """служебный метод, возвращающий кортеж с размерами локации"""
        return (self.size_x, self.size_y)


    def add_monster(self, name : str, hello : str, hp : int, x : int, y : int) -> str:
        """метод, реализующий добавление монстра name на клетку с координатами x,y и
        приветствием hello. Возвращает строку для клиента."""
        
        # Проверяем корректность имени данного монстра. Если некорр. - возвращаем соотв. диагностику
        if name not in cowsay.list_cows() and name != 'jgsbat' :
            return "1\nCannot add unknown monster"
        
        # проверяем наличие монстра по указанным координатам и получаем имя, если монстр есть
        flag = (x,y) in self.monsters
        if flag:
            prev_name = self.monsters[(x,y)].get_name()

        # Кладем в словарь монстров монстра с данным hello и name по ключу (x,y)
        self.monsters[(x,y)] = Monster(hello, name, hp)
        
        # формируем строку для игрока. Согласно протоколу, в первую строку, помимо
        # числа строк в сообщении нужно указать появившизся и исчезнувших монстров
        return_str = f"Added monster {name} to ({x}, {y}) saying {hello}"
        if flag:
            return_str += "\nReplaced the old monster"
            return_str = str(return_str.count('\n') + 1) + ' add ' + name + ' del ' + prev_name + '\n' + return_str
        else:
            return_str = str(return_str.count('\n') + 1) + ' add ' + name + '\n' + return_str

        return return_str


    def encounter(self, x : int, y : int) -> bool:
        """Метод, реализующий происшествие при встрече игрока с монстром
        Возвращает сообщение для клиента"""
        
        # Если по указанным координатам есть монстр, то запускаем приветсвие
        if (x,y) in self.monsters:
            return self.monsters[(x,y)].greetings()
        return ''


    def encounter_attack(self, x: int, y: int, name: str, damage : int) -> str:
        """Метод, реализующий атаку игрока на монстра"""
        
        if (x,y) not in self.monsters:
            return "1\nNo monster here"
            
        elif self.monsters[(x,y)].get_name() != name:
                return f"1\nNo monster {name} here"

        # если указанный монстр находится в текущей клетке, то запускам битву
        else:
            res = self.monsters[(x,y)].battle(damage)
            return_str = f"Attacked {res[2]}, damage {res[1]} hp\n"
            
            if res[0] == 0:
                return_str += f"{res[2]} died"
                # согласно протоколу, мы должны сообщить, что монстра больше нет
                return_str =  str(return_str.count('\n') + 1) + ' del ' + name + '\n' + return_str
                self.monsters.pop((x,y))
            else:
                return_str += f"{res[2]} now has {res[0]} hp"
                return_str = return_str = str(return_str.count('\n') + 1) + '\n' + return_str
            return return_str


    def get_monsters(self) -> list[str]:
        """Метод для получения списка доступных монстров"""
        
        res = []
        for coord_set in self.monsters:
            cur_monster = self.monsters[coord_set]
            res.append(cur_monster.get_name())
        return res
                

class Player:
    """Класс игроков"""
    def __init__(self, x : int = 0, y : int = 0, location : Location = Location()):
        """Инициализируем игрока: задаем локацию(Объект класса Location) и координаты точки спавна"""
        
        self.x = x
        self.y = y
        self.location = location


    #Метод для передвижения игрока. Возвращает кортеж с итоговыми координатами.
    def move(self, cmd : str) -> Tuple[int, int]:
        """Метод для передвижения игрока. Возвращает кортеж с итоговыми координатами.
        Меняем координаты в зависимости от команды с учетом того, что:
          1) нумерация клеток увеличивается сверху вниз, слева направо
          2) если игрок переходит границу, он появляется с обратной стороны поля"""
        
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

        # формируем сообщение для пользователя: куда подвинулся
        # и приветсвие монстра, если там где он оказался есть монстр
        return_str = f"Moved to ({self.x}, {self.y})"
        meeting = self.location.encounter(self.x, self.y)
        if meeting:
            return_str += '\n' + meeting

        return_str = str(return_str.count('\n') + 1) + '\n' + return_str
        return return_str


    def init_monster(self, name : str, hello : str, hp : int, x : int, y : int) -> Monster:
        """Метод для запуска процесса добавления нового монстра на текущую локацию игрока"""
        
        return self.location.add_monster(name, hello, hp, x, y)


    def attack(self, name:str, weapon : str = 'sword'):
        """Метод для запуска процесса битвы с монстром"""

        weapons = {'sword': 10, 'spear': 15, 'axe': 20}  
        if weapon not in weapons:
            return "1\nUnkown weapon"
        else:
            damage = weapons[weapon]
            return self.location.encounter_attack(self.x, self.y, name, damage)

    
    def available_monsters(self) -> list[str]:
        """Метод для получения списка достпных монстров"""
        return self.location.get_monsters()


# серверная часть

# словарь в формате {клиент}: {очередь запросов}
clients = {}


async def MUD(reader, writer):
    """Функция, реализуящая функциональности сервера"""

    # словарь доступных команд и соотв. им методов класса Player
    available_cmds = {'move': Player.move, 'attack' : Player.attack, 'addmon': Player.init_monster}

    # заводим для клиента объект класса Player и инициализируем очередь запросов
    player = Player()
    me = "{}:{}".format(*writer.get_extra_info('peername'))
    print(me, "Connected")
    clients[me] = asyncio.Queue()

    # создаем два задания: на чтение и на запись
    send = asyncio.create_task(reader.readline())
    receive = asyncio.create_task(clients[me].get())
    
    while not reader.at_eof():
        # обрабатываем выполненные задания с учетом того, что они могут закончиться одновременно
        done, pending = await asyncio.wait([send, receive], return_when=asyncio.FIRST_COMPLETED)
        for q in done:
            if q is send:
                # принимаем команду от клиента и разбиваем ее на аргументы
                send = asyncio.create_task(reader.readline())
                cur_rcv = q.result()
                parsed_cmd = shlex.split(cur_rcv.decode())

                # приводим все аргументы к соотв. типу из аннотаций и
                # аргумент - название команды на объект класса Player
                arg_ind = 1
                args_len = len(parsed_cmd)
                cur_method = available_cmds[parsed_cmd[0]]
                parsed_cmd[0] = player
                cur_annotations = cur_method.__annotations__
                
                for i in cur_annotations:
                    if arg_ind == args_len:
                        break
                    if parsed_cmd[arg_ind]:
                        parsed_cmd[arg_ind] = (cur_annotations[i](parsed_cmd[arg_ind]))
                        arg_ind += 1

                # Вызываем нужный метод с сформированным списком аргументов
                # кладем результат в очередь
                res = cur_method.__call__(*parsed_cmd)
                await clients[me].put(res)
               
            elif q is receive:
                # достаем результат из очереди и посылаем его клиенту
                receive = asyncio.create_task(clients[me].get())
                cur_rcv = q.result()
                writer.write((f"{cur_rcv}\n").encode())
                await writer.drain()

    # закрываем соединение
    send.cancel()
    receive.cancel()
    print(me, "DONE")
    del clients[me]
    writer.close()
    await writer.wait_closed()


async def main():
    """Запуск сервера"""
    
    server = await asyncio.start_server(MUD, '0.0.0.0', 1337)
    async with server:
        await server.serve_forever()

asyncio.run(main())

