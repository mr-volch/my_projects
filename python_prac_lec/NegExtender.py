class NegExt:
    def __neg__(self):
        try:
            _A = self.__class__(super().__neg__())
        except Exception:
            try:
                _A = self.__class__(super().__getitem__(slice(1,-1)))
            except Exception:
                return self.__class__(self)
            else:
                return _A
        else:
            return _A
