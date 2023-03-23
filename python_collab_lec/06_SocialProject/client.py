import cmd
import threading
import time
import readline

semop_answer = threading.Semaphore(value = 0)
semop_request = threading.Semaphore(value = 0)
answer = None
request = None


class simple(cmd.Cmd):

    def do_who(self, args):
        global request, answer
        request = 'who'
        semop_request.release()
        semop_answer.acquire()
        print(answer)

    def do_cows(self, args):
        global request, answer
        request = 'cows'
        semop_request.release()
        semop_answer.acquire()
        print(answer)

    def do_login(self, args):
        global request, answer
        request = (f'login {args}')
        semop_request.release()
        semop_answer.acquire()
        print(answer)

    def complete_login(self, text, line, begidx, endidx):
        global request, answer
        request = 'cows'
        semop_request.release()
        semop_answer.acquire()
        completion = answer.split()
        return completion

    def do_say(self, args):
        global request, answer
        request = f'say {args}'
        semop_request.release()
        semop_answer.acquire()
        print(answer)

    def complete_say(self, text, line, begidx, endidx):
        global request, answer
        request = 'who'
        semop_request.release()
        semop_answer.acquire()
        completion = answer.split()
        return completion

    def do_yield(self, args):
        global request, answer
        request = f'yield {args}'
        semop_request.release()
        semop_answer.acquire()
        print(answer)

    def do_quit(self, args):
        global request
        request = 'quit'
        semop_request.release()
        return True

    


def spam(cmdline, timeout, count):
    while True:
        global request, answer
        semop_request.acquire()
        print(request)
        if request == 'quit':
            break
        answer = request + '1'
        semop_answer.release()

    return True


cmdline = simple()
timer = threading.Thread(target=spam, args=(cmdline, 3, 10))
timer.start()
cmdline.cmdloop()
