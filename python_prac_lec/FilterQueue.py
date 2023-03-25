import asyncio

class FilterQueue(asyncio.Queue):

    @property
    def window(self):
        if self.empty():
            return None
        else:
            a = self._queue[0]
            return a

    async def __contains__(self, item):
        for i in self._queue:
            if item(i):
                return True
        return False

    def later(self):
        if self.empty():
            raise asyncio.QueueEmpty
        else:
            cur = self.get_nowait()
            self.put_nowait(cur)

    async def get(self, filt = None):
        if filt == None:
            return await super().get()
        i = 0
        if await self.__contains__(filt):
            while not filt(self._queue[i]):
                self.later()
        return await super().get()
