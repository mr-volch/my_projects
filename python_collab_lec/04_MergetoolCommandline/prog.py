import shlex
import readline
import cmd
import cowsay


def preproc_dict(fin_args, cur_args):
    a = 0
    while a < len(cur_args):
        if cur_args[a] in fin_args.keys():
            cur_assign = cur_args[a+1]
            if isinstance(fin_args[cur_args[a]], int):
                cur_assign = int(cur_assign)
            fin_args[cur_args[a]] = cur_assign
            a += 1
        a += 1
    return fin_args



class CowShell(cmd.Cmd):

    def do_list_cows(self, arg):
        cur_args = parse(arg)
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('list_cows')
        else:
            fin_args = {'cow_path': cowsay.COW_PEN}
            try:
                fin_args = preproc_dict(fin_args, cur_args)
            except:
                print("bad format. Type -h to see the correct cmd format")
            else:
                print(cowsay.list_cows(*tuple(fin_args.values())))
                           

    def help_list_cows(self):
        print("""format: list_cows [cow_path <path_to_directory>] (if cow_path is not given, list of standard cows will be writen")
    Lists all cow file names in the given directory""")


    def do_make_bubble(self, arg):
        cur_args = parse(arg)
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('make_bubble')
        else:
            if 'text' not in cur_args:
                print("bad format: no message attribute. Type -h to see the correct cmd format")
            else:
                fin_args = {'text': None, 'brackets' : cowsay.THOUGHT_OPTIONS['cowsay'], 'width' : 40, 'wrap_text' : True}
                try:
                    fin_args = preproc_dict(fin_args, cur_args)
                except:
                    print("bad format. Type -h to see the correct cmd format")
                else:
                    print(cowsay.make_bubble(*tuple(fin_args.values())))


    def complete_make_bubble(self, text, line, begidx, endidx):
        target_bubble = ['-h', '--help', 'text', 'width']
        target_text = ["Hello", "Moooo", "Meow"]
        target_width = ["30", "40", "50"]

        line = line.split()

        match line[-1]:
            case 'text':
                completions = target_text
            case 'width':
                completions = target_width
            case _:
                completions = target_bubble

        return completions


    def help_make_bubble(self):
        print("""format: make_bubble text <string> [width <int>] [wrap_text <bool>] (defaults: width = 40, wrap_text = True)
    Wraps text is wrap_text is true, then pads text and sets inside a bubble, which width is given in cmd.
    This is the text that appears above the cows""")


    def do_cowsay(self, arg):
        cur_args = parse(arg)
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('cowsay')
        else:
            if 'message' not in cur_args:
                print("bad format: no message attribute. Type -h to see the correct cmd format")
            else:
                fin_args = {'message': None, 'cow': 'default', 'preset': None, 'eyes' : 'oo', 'tongue' : '  '}
                try:
                    fin_args = preproc_dict(fin_args, cur_args)
                except:
                    print("bad format. Type -h to see the correct cmd format")
                else:
                    print(cowsay.cowsay(*tuple(fin_args.values())))


    def complete_cowsay(self, text, line, begidx, endidx):
        target_cowsay = ['-h', '--help', 'message', 'cow', 'eyes', 'tongue']
        target_messages = ["Hello", "Moooo", "Meow"]
        target_cows = ["turtle", "dragon-and-cow", "kitten"]
        target_eyes = ["==", "XX", "**"]
        target_tongues = ["U", "\/", "L"]

        line = line.split()

        match line[-1]:
            case 'message':
                completions = target_messages
            case 'cow':
                completions = target_cows
            case 'eyes':
                completions = target_eyes
            case 'tongue':
                completions = target_tongues
            case _:
                completions = target_cowsay

        return completions


    def help_cowsay(self):
        print("""format: cowsay message <string> [cow <string>] [eyes <string>] [tongue <string>]
    Returns the resulting cowsay string
    param message: The message to be displayed
    param cow: the available cows can be found by calling list_cows
    param eyes: eye_string
    param tongue: tongue_string""")


    def do_cowthink(self, arg):
        cur_args = parse(arg)
        if '-h' in cur_args or '--help' in cur_args:
            self.do_help('cowthink')
        else:
            if 'message' not in cur_args:
                print("bad format: no message attribute. Type -h to see the correct cmd format")
            else:
                fin_args = {'message': None, 'cow': 'default', 'preset': None, 'eyes' : 'oo', 'tongue' : '  '}
                try:
                    fin_args = preproc_dict(fin_args, cur_args)
                except:
                    print("bad format. Type -h to see the correct cmd format")
                else:
                    print(cowsay.cowthink(*tuple(fin_args.values())))


    def complete_cowthink(self, text, line, begidx, endidx):
        return self.complete_cowsay(text, line, begidx, endidx)


    def help_cowthink(self):
        print("""format: cowthink message <string> [cow <string>] [eyes <string>] [tongue <string>]
    Returns the resulting cowthink string
    param message: The message to be displayed
    param cow: the available cows can be found by calling list_cows
    param eyes: eye_string
    param tongue: tongue_string""")




def parse(arg):
    return (tuple(shlex.split(arg)))


if __name__ == '__main__':
    CowShell().cmdloop()
