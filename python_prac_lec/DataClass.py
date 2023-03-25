def sloter(fields, default):
    class s:
        __slots__ = list(fields)
        slots_iter = __slots__.__iter__()
        val_def = default

        def __init__(self):
            for i in range(len(self.__slots__)):
                cur_attribute = self.__slots__[i]
                setattr(self,cur_attribute,self.val_def)

        def __iter__(self):
            cur_vals = []
            for i in self.__slots__:
                cur_vals.append(getattr(self,i))
            return cur_vals.__iter__()

        def __delattr__(self, attrname):
            if attrname in self.__slots__:
                setattr(self,attrname,self.val_def)
            else:
                raise AttributeError

    return s
