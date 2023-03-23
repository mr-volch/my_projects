class Num:        
    def __set__(self, obj, val):
        if hasattr(val, 'real'):
            obj._num = val
        elif hasattr(val, '__len__'):
            obj._num = len(val)
        

            

    def __get__(self, obj, cls):
        try:
            return obj._num
        except AttributeError:
            return 0

    def __delete__(self, obj):
        pass

import sys
exec(sys.stdin.read())
