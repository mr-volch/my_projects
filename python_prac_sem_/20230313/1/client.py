import asyncio
import cmd
import shlex


def make_args(available_cmds : dict[str, callable], cmd : list[str]):
    """функция, делающая кортеж с параметрами для запускаемых команд
    аргументы: 1) словарь доступных команд available_cmds в формате {<команда>:<класс>.{<полe>.}<метод>}
               2) сама по себе команда (а именно список слов ее составляющих, полученый из .split())
    возвращает: 1 - плохая команда, 2 - плохие аргументы, кортеж - успех. кортеж для вызова"""
    
    # Вычленяем нужный метод класса (кортеж вида : (<Название класса(str)>, <метод класса>))
    try:
        cur_cmd = available_cmds[cmd[0]]
    except:
        return 1

    #В зависимости от того, для какого класса делаем, заполняем начальные позиции аргументов
    match cur_cmd:
        case "":
            #Здесь добавляем только команду (напр. 'up')
            args = [cmd[0]]
        case cmd_args:
            #считаем ожидаемую длину списка аргументов, создаем пустой список
            #и инициализируем его первый элемент именем текущей команды
            args_len = 1
            for i in cmd_args:
                args_len += len(cmd_args[i])
            args = ['' for i in range(args_len)]
            args[0] = cmd[0]
            #Пробегаемся по всем поданным аргументам и заносим их
            #в список согласно порядку, установленному в cmd_args
            n = 0
            try:
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
            except AttributeError:
                return 2

    #возвращаем список с аргументами
    return args


class network_link:
    """служебный класс для обслуживания "соединения" с сервером
    хранит объекты reader и writer, ассоциированные с текущим соединением"""
    
    reader = None
    writer = None
    
    async def set_link(reader, writer):
        """служебный метод, для задания объектов reader и writer"""
        
        network_link.reader = reader
        network_link.writer = writer



    async def caller(fin_args : list[str]):
        """Все взаимодействие с севрером происходит здесь. fin_args - команда для сервера"""

        # если в fin_args - код ошибки, выводим диагностику 
        match fin_args:
            case 1:
                print("invalid command")
            case 2:
                print("invalid arguments")
            # в случае успешно составленной команды, начинаем взаимодействие с севрером
            case _:
                # Удаляем все пустые аргументы из конца команды:
                # в них сервер подставит знаения по умолчанию
                while fin_args[-1] == '':
                    fin_args.pop(-1)

                # Заключаем все слова в команде в ковычки, чтобы сервер
                # мог распознавать многословные аргументы и шлем команду серверу
                send_box = ("'" + "' '".join(fin_args) + "'" + '\n').encode()
                network_link.writer.write(send_box)
                await network_link.writer.drain()

                # получаем ответ от сервера
                receive_from_server = asyncio.create_task(network_link.reader.readline())
                q = await receive_from_server

                # Начинаем анализировать первую строку: согласно протоколу
                # первый элемент первой строки - кол-во строк, которые сервер прислал (помимо первой)
                q = q.decode().split()
                strings_amount = int(q[0])

                # согласно протоколу: если в первой строке есть что-то кроме числа строк,
                # то в оставшейся части строки указана информация о появлении/исчезновении монстров
                if len(q) > 1:
                    # читаем строку с информацией о монстрах и изменяем
                    # словарь с существующими монстрами класса game_shell
                    for i in range(1, len(q), 2):
                        if q[i] == 'add':
                            game_shell.current_monsters.setdefault(q[i+1], 0)
                            game_shell.current_monsters[q[i+1]] += 1
                        if q[i] == 'del':
                            game_shell.current_monsters[q[i+1]] -= 1
                            if game_shell.current_monsters[q[i+1]] == 0:
                                game_shell.current_monsters.pop(q[i+1])

                #Согласно протоколу, все оставшиеся строки в посылке предназначены для вывода пользователю
                for i in range(strings_amount):
                    receive_from_server = asyncio.create_task(network_link.reader.readline())
                    q = await receive_from_server
                    print(q.decode(), end = '')
    
              
class game_shell(cmd.Cmd):
    """Класс - текстовый интерфейс на стороне клиента"""

    # словарь доступных команд. Формат:
    # команда: {название позиционного аргумента : (индексы на которых располагаются значения для данного аргумента)}
    available_cmds = {'up': '', 'down': '', 'left': '', 'right': '',
                      'attack' : {'attack' : (1,), 'with':(2,)},
                      'addmon': {'addmon' : (1,), 'hello' : (2,), 'hp' : (3,), 'coords' : (4,5)}}
    #переменыее для пролистывания вариантов в completion
    cur_monster_pos = 0
    cur_weapon_pos = 0
    current_monsters = {}

    # перегружаем два метода из класса cmd.Cmd, чтобы сделать их
    # работу совместимой с asyncio. места изменени помечены комментариями
    async def cmdloop(self, intro=None):
        """Repeatedly issue a prompt, accept input, parse an initial prefix
        off the received input, and dispatch to action methods, passing them
        the remainder of the line as argument.
        """
    
        self.preloop()
        if self.use_rawinput and self.completekey:
            try:
                import readline
                self.old_completer = readline.get_completer()
                readline.set_completer(self.complete)
                readline.parse_and_bind(self.completekey+": complete")
            except ImportError:
                pass
        try:
            if intro is not None:
                self.intro = intro
            if self.intro:
                self.stdout.write(str(self.intro)+"\n")
            stop = None
            while not stop:
                if self.cmdqueue:
                    line = self.cmdqueue.pop(0)
                else:
                    if self.use_rawinput:
                        try:
                            line = input(self.prompt)
                        except EOFError:
                            line = 'EOF'
                    else:
                        self.stdout.write(self.prompt)
                        self.stdout.flush()
                        line = self.stdin.readline()
                        if not len(line):
                            line = 'EOF'
                        else:
                            line = line.rstrip('\r\n')
                line = self.precmd(line)
                #создаем задание и awaitим его
                cur_task = asyncio.create_task(self.onecmd(line))
                stop = await cur_task
                stop = self.postcmd(stop, line)
            self.postloop()
        finally:
            if self.use_rawinput and self.completekey:
                try:
                    import readline
                    readline.set_completer(self.old_completer)
                except ImportError:
                    pass

         
    async def onecmd(self, line):
        """Interpret the argument as though it had been typed in response
        to the prompt.
        This may be overridden, but should not normally need to be;
        see the precmd() and postcmd() methods for useful execution hooks.
        The return value is a flag indicating whether interpretation of
        commands by the interpreter should stop.
        """
        cmd, arg, line = self.parseline(line)
        if not line:
            return self.emptyline()
        if cmd is None:
            return self.default(line)
        self.lastcmd = line
        if line == 'EOF' :
            self.lastcmd = ''
        if cmd == '':
            return self.default(line)
        else:
            try:
                func = getattr(self, 'do_' + cmd)
            except AttributeError:
                return self.default(line)
            #место вызова функции do_<cmd>. Здесь на нее ссылается func
            #создаем задание и awaitим его
            cur_task = asyncio.create_task(func(arg))
            res = await cur_task
            return res


    # Обработка команд передвижения пользователя:
    # заполняем команду в формате move <direction>
    # и передаем её функции, отвечающей за взаимод. с сервером    
    async def do_up(self, args):
        """обработка команды up"""
        
        cur_args = shlex.split(args)
        cur_args.insert(0, 'up')
        fin_args = make_args(game_shell.available_cmds, cur_args)
        fin_args.insert(0, "move")
        await network_link.caller(fin_args)


    async def do_down(self, args):
        """обработка команды down"""
        
        cur_args = shlex.split(args)
        cur_args.insert(0, 'down')
        fin_args = make_args(game_shell.available_cmds, cur_args)
        fin_args.insert(0, "move")
        await network_link.caller(fin_args)

        
    async def do_right(self, args):
        """обработка команды right"""
        
        cur_args = shlex.split(args)
        cur_args.insert(0, 'right')
        fin_args = make_args(game_shell.available_cmds, cur_args)
        fin_args.insert(0, "move")
        await network_link.caller(fin_args)


    async def do_left(self, args):
        """обработка команды left"""
        
        cur_args = shlex.split(args)
        cur_args.insert(0, 'left')
        fin_args = make_args(game_shell.available_cmds, cur_args)
        fin_args.insert(0, "move")
        await network_link.caller(fin_args)


    async def do_attack(self, args):
        """обработка команды attack"""

        #обнуляем счетчики для completion
        game_shell.cur_weapon_pos = 0
        game_shell.cur_monster_pos = 0
        
        cur_args = shlex.split(args)       
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('attack')
        else:
            cur_args.insert(0, 'attack')
            fin_args = make_args(game_shell.available_cmds, cur_args)
            await network_link.caller(fin_args)


    def help_attack(self, args):
        """help message для команды attack"""
        
        print("""format: attack <monster_name >with <weapon_name>
    attack monster with name <monster_name> with weapon <weapon_name>, if rhere is a monster on your current position""")


    def complete_attack(self, text, line, begidx, endidx):
        """автодополнение в режиме "пролистывания" для команды attack"""

        # заводим списки с возможными дополениями
        target_attack = list(game_shell.current_monsters.keys())
        target_with = ["sword", "spear", "axe"]

        line = line.split()

        # циклично по длине списка выводим его элементы в completions
        if line[-1] in target_with or line[-1] == 'with':
            completions = [target_with[game_shell.cur_weapon_pos]]
            game_shell.cur_weapon_pos = ((game_shell.cur_weapon_pos + 1) % len(target_with))
        else:
            completions = [target_attack[game_shell.cur_monster_pos]]
            game_shell.cur_monster_pos = ((game_shell.cur_monster_pos + 1) % len(target_attack))
            
        return completions


    async def do_addmon(self, args):
        """обработка команды addmon"""
        
        cur_args = shlex.split(args)

        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('addmon')
        else:
            cur_args.insert(0, 'addmon')
            fin_args = make_args(game_shell.available_cmds, cur_args)
            await network_link.caller(fin_args)


    def help_addmon(self):
        """help message для команды addmon"""
        
        print("""format: addmon <monster_name> hello <hello_string> hp <hitpoints> coords <x> <y>
    adds a new monster to player`s location
    param <monster_name>: one of available ascii arts for python-cowsay module
    param hello: hello string
    param hp: monster`s hit points
    param coords: coordinates of exect place on location for monster""")


    def complete_addmon(self, text, line, begidx, endidx):
        """автодополнение для команды addmon в режиме вывода всех предложений для completion"""
        
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
                completions = target_coords
            case 'hp':
                completions = target_hp
            case 'hello':
                completions = target_hello
            case _:
                completions = target_cows

        return completions


async def game_loop():
    """Запуск игры"""
    
    print("<<< Welcome to Python-MUD 0.2 >>>")
    # устанавливаем соединение
    reader, writer = await asyncio.open_connection('0.0.0.0', 1337)
    task_link = asyncio.create_task(network_link.set_link(reader, writer))
    await task_link
    # запускаем TUI
    task_game = asyncio.create_task(game_shell().cmdloop())
    await task_game

   
asyncio.run(game_loop())
