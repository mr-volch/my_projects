class Omnibus:
    _dict_attr = {}

    def __setattr__(self, a, n):
        if a[0] != '_':
            Omnibus._dict_attr.setdefault(a, set())
            Omnibus._dict_attr[a].add(self)

    def __getattr__(self, a):
        return len(Omnibus._dict_attr[a])

    def __delattr__(self, a):
        if a[0] != '_' and a in Omnibus._dict_attr and self in Omnibus._dict_attr[a]:
            Omnibus._dict_attr[a].remove(self)


import sys
exec(sys.stdin.read())
