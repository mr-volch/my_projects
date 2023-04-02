"""Server module.

Этот модуль реализует серверную часть проекта MUD.
Здесь происходит выполнение готовой команды, присланной от пользователя,
после чего происходит посылка результата пользователю или широковещательного сообщения.
"""

import cowsay
import asyncio
import shlex
from io import StringIO


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
    """Класс монстров."""

    def __init__(self, hello: str, name: str, hp: int):
        """Инициализируем нового монстра: вносим приветствие и имя."""
        self.hello = hello
        self.name = name
        self.hp = hp

    def get_name(self) -> str:
        """Getter имени монстра."""
        return self.name

    def greetings(self):
        """Метод, реализующий встречу с монстром.

        Возвращает приветствие, оформленное модулем cowsay.
        """
        if self.name == 'jgsbat':
            return cowsay.cowsay(self.hello, cowfile=jgsbat)
        return cowsay.cowsay(self.hello, cow=self.name)

    def battle(self, damage: int) -> tuple[int]:
        """Метод, реализующий битву с монстром.

        Примает: наносимый урон.
        Возвращает: кортеж вида: (остаток жизней, нанесенный урон, имя).
        """
        prev_hp = self.hp
        self.hp -= damage
        if self.hp <= 0:
            return (0, prev_hp, self.name)
        else:
            return (self.hp, damage, self.name)


class Location:
    """Класс локаций(карт)."""

    def __init__(self, size_x: int = 10, size_y: int = 10):
        """Иницализируем новую локацию: задаем размеры и заводим пустой словарь монстров."""
        self.size_x = size_x
        self.size_y = size_y
        # формат словаря: ключи - кортежи с координатами монстров (x,y),
        #                 значения - объекты класса Monster
        self.monsters = {}

    def get_size(self):
        """Служебный метод, возвращающий кортеж с размерами локации."""
        return (self.size_x, self.size_y)

    def add_monster(self, name: str, hello: str, hp: int, x: int, y: int) -> str:
        """Метод, реализующий добавление монстра name на клетку с координатами x,y.

        Возвращает строку для клиента.
        """
        # Проверяем корректность имени данного монстра. Если некорр. - возвращаем соотв. диагностику
        if name not in cowsay.list_cows() and name != 'jgsbat':
            return ("Cannot add unknown monster", 0)
        # проверяем наличие монстра по указанным координатам и получаем имя, если монстр есть
        flag = (x, y) in self.monsters
        # Кладем в словарь монстров монстра с данным hello и name по ключу (x,y)
        self.monsters[(x, y)] = Monster(hello, name, hp)

        # формируем строку для игрока. Согласно протоколу, в первую строку, помимо
        # числа строк в сообщении нужно указать появившизся и исчезнувших монстров
        return_str = f"added monster {name} to ({x}, {y}) saying {hello} with {hp} hp"
        if flag:
            return_str += "\nReplaced the old monster"

        return (return_str, 1)

    def encounter(self, x: int, y: int) -> bool:
        """Метод, реализующий происшествие при встрече игрока с монстром.

        Возвращает сообщение для клиента
        """
        # Если по указанным координатам есть монстр, то запускаем приветсвие
        if (x, y) in self.monsters:
            return self.monsters[(x, y)].greetings()
        return ''

    def encounter_attack(self, x: int, y: int, name: str, weapon: str) -> str:
        """Метод, реализующий атаку игрока на монстра."""
        weapons = {'sword': 10, 'spear': 15, 'axe': 20}
        if weapon not in weapons:
            return ("Unkown weapon", False)

        if (x, y) not in self.monsters:
            return ("No monster here", False)
        elif self.monsters[(x, y)].get_name() != name:
            return (f"No monster {name} here", False)

        # если указанный монстр находится в текущей клетке, то запускам битву
        else:
            res = self.monsters[(x, y)].battle(weapons[weapon])
            return_str = f"attacked {res[2]} with {weapon}, damage {res[1]} hp\n"

            if res[0] == 0:
                return_str += f"{res[2]} died"
                self.monsters.pop((x, y))
            else:
                return_str += f"{res[2]} now has {res[0]} hp"

            return (return_str, True)

    def get_monsters(self) -> list[str]:
        """Метод для получения списка доступных монстров."""
        res = []
        for coord_set in self.monsters:
            cur_monster = self.monsters[coord_set]
            res.append(cur_monster.get_name())
        return res


class Player:
    """Класс игроков."""

    def __init__(self, x: int = 0, y: int = 0, location: Location = Location()):
        """Инициализируем игрока: задаем локацию(Объект класса Location) и коорд. точки спавна."""
        self.x = x
        self.y = y
        self.location = location

    # Метод для передвижения игрока. Возвращает кортеж с итоговыми координатами.
    def move(self, cmd: str) -> tuple[int, int]:
        """Метод для передвижения игрока. Возвращает кортеж с итоговыми координатами.

        Меняем координаты в зависимости от команды с учетом того, что:
          1) нумерация клеток увеличивается сверху вниз, слева направо.
          2) если игрок переходит границу, он появляется с обратной стороны поля.
        """
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

        return_str = return_str
        return (return_str,)

    def init_monster(self, name: str, hello: str, hp: int, x: int, y: int) -> Monster:
        """Метод для запуска процесса добавления нового монстра на текущую локацию игрока."""
        return self.location.add_monster(name, hello, hp, x, y)

    def attack(self, name: str, weapon: str = 'sword'):
        """Метод для запуска процесса битвы с монстром."""
        return self.location.encounter_attack(self.x, self.y, name, weapon)

    def available_monsters(self, *args) -> list[str]:
        """Метод для получения списка доступных монстров."""
        return (' '.join(self.location.get_monsters()), )

    def say(self, speach: str):
        """Метод для формирования сообщения в чат."""
        return ('said: ' + speach, True)


# серверная часть

# словарь в формате {клиент}: {очередь запросов}
clients = {}


async def MUD(reader, writer):
    """Функция, реализуящая функциональности сервера.

    Организует запуска на выполнения принемаемых от пользователя команд,
    а также посылает ответы пользователю или широковещательные сообщения.
    """
    to_login = asyncio.create_task(reader.readline())
    res = await to_login
    name = res.decode()[:-1]
    if name in clients:
        writer.write(("sorry").encode())
        await writer.drain()
        return False
    else:
        writer.write(("hello").encode())
        await writer.drain()
        print(name, "Connected")
        for cur_name in clients:
            await clients[cur_name].put(f"{name} has connected")
        clients[name] = asyncio.Queue()

    class cmd_holder:
        def __init__(self, func: callable, is_broadcast: bool):
            self.target = func
            self.broadcast = is_broadcast

        def get_target(self):
            return self.target

        def get_broadcast(self):
            return self.broadcast

    # словарь доступных команд и соотв. им методов класса Player
    available_cmds = {'move': cmd_holder(Player.move, False),
                      'attack': cmd_holder(Player.attack, True),
                      'addmon': cmd_holder(Player.init_monster, True),
                      'sayall': cmd_holder(Player.say, True),
                      'monsters': cmd_holder(Player.available_monsters, False)}
    # заводим для клиента объект класса Player
    player = Player()

    # создаем два задания: на чтение и на запись
    send = asyncio.create_task(reader.readline())
    receive = asyncio.create_task(clients[name].get())

    while not reader.at_eof():
        # обрабатываем выполненные задания с учетом того, что они могут закончиться одновременно
        done, pending = await asyncio.wait([send, receive], return_when=asyncio.FIRST_COMPLETED)
        for q in done:
            if q is send:
                # принимаем команду от клиента и разбиваем ее на аргументы
                send = asyncio.create_task(reader.readline())
                cur_rcv = q.result()
                parsed_cmd = shlex.split(cur_rcv.decode())

                if parsed_cmd[0] == 'quit':
                    break

                # приводим все аргументы к соотв. типу из аннотаций и
                # аргумент - название команды на объект класса Player
                arg_ind = 1
                args_len = len(parsed_cmd)
                cur_cmd = available_cmds[parsed_cmd[0]]
                cur_method = cur_cmd.get_target()
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

                if cur_cmd.get_broadcast() and res[1]:
                    for cur_name in clients:
                        if cur_name != name:
                            await clients[cur_name].put(f'{name} {res[0]}')
                await clients[name].put(res[0])

            elif q is receive:
                # достаем результат из очереди и посылаем его клиенту
                receive = asyncio.create_task(clients[name].get())
                cur_rcv = q.result()
                print(cur_rcv)
                writer.write((f"{cur_rcv}").encode())
                await writer.drain()

        else:
            continue
        break

    # закрываем соединение
    send.cancel()
    receive.cancel()

    print(name, "DONE")
    writer.write("quit".encode())
    await writer.drain()

    del clients[name]
    writer.close()
    await writer.wait_closed()

    for cur_name in clients:
        await clients[cur_name].put(f"{name} has disconnected")

    return True


async def main():
    """Запуск сервера."""
    server = await asyncio.start_server(MUD, '0.0.0.0', 1337)
    async with server:
        await server.serve_forever()
