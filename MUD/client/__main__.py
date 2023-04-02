"""Client module.

This module is a server part of the MUD project
Parsing is done here and asynchronous connection with server is done
"""
import threading
import sys
from .client import game_shell
from .client import communicate

cmdline = game_shell()
try:
    name = sys.argv[1]
except:
    name = input("Please, insert nickname: ")

communication = threading.Thread(target=communicate, args=(cmdline, name))
communication.start()
