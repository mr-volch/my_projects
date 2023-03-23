def objcount(cls):
    class new_cls(cls):
        counter = 0
        def __init__(self):
            new_cls.counter += 1

        def __del__(self):
            new_cls.counter -= 1

    return new_cls

import sys
exec(sys.stdin.read())
