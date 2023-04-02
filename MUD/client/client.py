"""Client module.

This module is a server part of the MUD project
Parsing is done here and asynchronous connection with server is done
"""
import cmd
import shlex
import readline
import socket
import threading


def make_args(available_cmds: dict[str, callable], cmd: list[str]):
    """Функция, делающая кортеж с параметрами для запускаемых команд.

    аргументы:
    1) словарь доступных команд available_cmds в формате {<команда>:<класс>.{<полe>.}<метод>}
    2) сама по себе команда (а именно список слов ее составляющих, полученый из .split())
    возвращает: 1 - плохая команда, 2 - плохие аргументы, кортеж - успех. кортеж для вызова
    """
    # Вычленяем нужный метод класса (кортеж вида : (<Название класса(str)>, <метод класса>))
    try:
        cur_cmd = available_cmds[cmd[0]]
    except KeyError:
        return 1

    # В зависимости от того, для какого класса делаем, заполняем начальные позиции аргументов
    match cur_cmd:
        case "":
            # Здесь добавляем только команду (напр. 'up')
            args = [cmd[0]]
        case cmd_args:
            # считаем ожидаемую длину списка аргументов, создаем пустой список
            # и инициализируем его первый элемент именем текущей команды
            args_len = 1
            try:
                for i in cmd_args:
                    args_len += len(cmd_args[i])
            except TypeError:
                return 2
            args = ['' for i in range(args_len)]
            args[0] = cmd[0]
            # Пробегаемся по всем поданным аргументам и заносим их
            # в список согласно порядку, установленному в cmd_args
            n = 0
            try:
                while n < len(cmd):
                    if cmd[n] in cmd_args.keys():
                        args_amount = len(cmd_args[cmd[n]])
                        cur_arg = cmd_args[cmd[n]]
                        n += 1
                    else:
                        raise IndexError

                    for j in range(args_amount):
                        args[cur_arg[j]] = cmd[n]
                        n += 1
            except IndexError:
                return 2

    # возвращаем список с аргументами
    return args


semop_answer = threading.Semaphore(value=0)
semop_request = threading.Semaphore(value=0)
fin_semop = threading.Semaphore(value=0)
answer = ''
request = ''
waiting = False


class game_shell(cmd.Cmd):
    """Класс - текстовый интерфейс на стороне клиента."""

    # Вступлние
    intro = "List of avialable comands is available on 'Tab' key or via help comand"

    # словарь доступных команд. Формат:
    # команда: {название позиц. аргумента: (индексы значений для данного аргумента)}
    available_cmds = {'up': '', 'down': '', 'left': '', 'right': '',
                      'attack': {'attack': (1, ), 'with': (2, )},
                      'addmon': {'addmon': (1, ), 'hello': (2, ), 'hp': (3, ), 'coords': (4, 5)},
                      'sayall': {'sayall': (1, )}}
    # переменыее для пролистывания вариантов в completion
    cur_monster = ''
    cur_weapon_pos = 0
    cur_monster_pos = 0

    def send_request(self, fin_args):
        """Формирует строку для посылки серверу и поднятие семафора для запросов."""
        global request

        match fin_args:
            case 1:
                print("invalid command, insert help to see comand list")
            case 2:
                print("invalid arguments, insert {<cmd_name> -h|--help} to see help message")
            # в случае успешно составленной команды, начинаем взаимодействие с севрером
            case _:
                # Удаляем все пустые аргументы из конца команды:
                # в них сервер подставит знаения по умолчанию
                while fin_args[-1] == '':
                    fin_args.pop(-1)
                request = ("'" + "' '".join(fin_args) + "'" + '\n').encode()
                semop_request.release()

    # Обработка команд передвижения пользователя:
    # заполняем команду в формате move <direction>
    # и передаем её функции, отвечающей за взаимод. с сервером
    def args_preproc(self, args, cur_cmd):
        """Формирование списка аргументов команды в нужном порядке."""
        cur_args = shlex.split(args)
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help(cur_cmd)
            return None

        cur_args.insert(0, cur_cmd)
        fin_args = make_args(game_shell.available_cmds, cur_args)
        if cur_cmd in {'up', 'down', 'right', 'left'}:
            fin_args.insert(0, "move")
        return fin_args

    def do_up(self, args):
        """Обработка команды up."""
        fin_args = self.args_preproc(args, 'up')
        self.send_request(fin_args)

    def do_down(self, args):
        """Обработка команды down."""
        fin_args = self.args_preproc(args, 'down')
        self.send_request(fin_args)

    def do_right(self, args):
        """Обработка команды right."""
        fin_args = self.args_preproc(args, 'right')
        self.send_request(fin_args)

    def do_left(self, args):
        """Обработка команды left."""
        fin_args = self.args_preproc(args, 'left')
        self.send_request(fin_args)

    def do_attack(self, args):
        """Обработка команды attack."""
        # обнуляем счетчики для completion
        game_shell.cur_weapon_pos = 0
        game_shell.cur_monster_pos = 0

        fin_args = self.args_preproc(args, 'attack')
        if fin_args != None:
            self.send_request(fin_args)

    def help_attack(self, args):
        """Help message для команды attack."""
        print("""format: attack <monster_name >with <weapon_name>
    attack monster with name <monster_name> with weapon <weapon_name>,
    if rhere is a monster on your current position
Autocompletion via 'Tab' key is available""")

    def complete_attack(self, text, line, begidx, endidx):
        """Автодополнение в режиме "пролистывания" для команды attack."""
        # заводим списки с возможными дополениями
        target_with = ["sword", "spear", "axe"]

        line = line.split()

        # циклично по длине списка выводим его элементы в completions
        if line[-1] in target_with or line[-1] == 'with':
            completions = [target_with[game_shell.cur_weapon_pos]]
            game_shell.cur_weapon_pos = ((game_shell.cur_weapon_pos + 1) % len(target_with))
        else:
            global request, answer, waiting
            waiting = True
            self.send_request(['monsters'])
            semop_answer.acquire()
            completions = answer.split()

        return completions

    def do_addmon(self, args):
        """Обработка команды addmon."""
        fin_args = self.args_preproc(args, 'addmon')
        if fin_args != None:
            self.send_request(fin_args)

    def help_addmon(self):
        """Help message для команды addmon."""
        print("""format: addmon <monster_name> hello <hello_string> hp <hitpoints> coords <x> <y>
    adds a new monster to player`s location
    param <monster_name>: one of available ascii arts for python-cowsay module
    param hello: hello string
    param hp: monster`s hit points
    param coords: coordinates of exect place on location for monster
Autocompletion via 'Tab' key is available""")

    def complete_addmon(self, text, line, begidx, endidx):
        """Автодополнение для команды addmon в режиме вывода всех предложений для completion."""
        target_addmon = ["-h", "--help", "turtle", "kitten", "jgsbat"]
        target_cows = ["hello", "hp", "coords"]
        target_coords = ["0 0", "1 1", "2 2"]
        target_hp = ["40", "50", "60"]
        target_hello = ["hello!", "'you will be defeated!'", "'there is no way you beat me!'"]

        line = line.split()
        match line[-1]:
            case 'addmon':
                completions = target_addmon
            case 'coords':
                completions = target_coords
            case 'hp':
                completions = target_hp
            case 'hello':
                completions = target_hello
            case _:
                completions = target_cows

        return completions

    def do_sayall(self, args):
        """Обработка команды sayall."""
        fin_args = self.args_preproc(args, 'sayall')
        print(fin_args)
        self.send_request(fin_args)

    def do_quit(self, args):
        """Обработка команды quit."""
        self.send_request(['quit'])
        return True


def my_read(cmdline, sock):
    """Функция, читающая из сокета."""
    global answer, waiting
    while True:
        res = sock.recv(4096)
        res = res.decode()
        if res == 'quit':
            print("READER DONE")
            sock.send('bye'.encode())
            fin_semop.release()
            break

        answer = res
        if waiting:
            semop_answer.release()
            waiting = False
        else:
            print(f"\n{answer}\n{cmdline.prompt}{readline.get_line_buffer()}", end="", flush=True)
    return True


def my_write(cmdline, sock):
    """Функция, пишущая в сокет."""
    global request
    while True:
        semop_request.acquire()
        sock.send(request)
        if request == b"'quit'\n":
            print("WRITER DONE")
            fin_semop.release()
            break
    return True


def communicate(cmdline, name):
    """Функция, открывающая сокет и запускающая треды для читателя, писателя и shell."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 1337))
    sock.send((f"{name}\n").encode())
    res = sock.recv(4096)

    if res == b'hello':
        print("<<< Welcome to Python-MUD 1.1 >>>")
        reader = threading.Thread(target=my_read, args=(cmdline, sock))
        writer = threading.Thread(target=my_write, args=(cmdline, sock))
        shell = threading.Thread(target=cmdline.cmdloop)

        writer.start()
        shell.start()
        reader.start()

        fin_semop.acquire()
        fin_semop.acquire()
        print("SOCKET DONE")
    else:
        print(f"Nickname {name} is allready taken, please log in with another nickname")

    sock.close()
    return True
