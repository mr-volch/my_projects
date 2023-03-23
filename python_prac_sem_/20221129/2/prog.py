from inspect import get_annotations as ann
import types


class check(type):

    def create_new(ns):
        def f(self):
            for i in ns['__annotations__']:
                try:
                    tmp = getattr(self, i)
                except:
                    return False
                if type(ns['__annotations__'][i]) == types.GenericAlias:
                    if isinstance(tmp, ns['__annotations__'][i].__origin__):
                        continue
                    return False
                elif isinstance(tmp, ns['__annotations__'][i]):
                    continue
                else:
                    return False
            return True
        return f

    @staticmethod
    def __new__(metacls, name, parents, ns, **kwds):
        ns['check_annotations'] = metacls.create_new(ns)
        return super().__new__(metacls, name, parents, ns)

import sys
exec(sys.stdin.read())
