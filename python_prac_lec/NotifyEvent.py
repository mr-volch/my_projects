import asyncio

class NotifyEvent(asyncio.Event):
    _set_count = 0
    name = None

    def __init__(self, *args):
        self.E = asyncio.Event()
        return super().__init__(*args)

    def set(self, name = None):
        if self._set_count == 0:
            self._set_count += 1
        else:
            super().clear()
        self.name = name
        self.E.set()
        return super().set()

    async def wait(self):
        await super().wait()
        self.E.clear()
        return self.name

async def task(name, notify):
    my = 0
    others = 0
    while cur := await notify.wait():
        await notify.E.wait()
        if name == cur:
            my += 1
            print(name, ': ', my, ' / ', others, sep = '')
        else:
            others += 1
