#!/usr/bin/env python

""" client.py - Echo client for sending/receiving C-like structs via socket

References:
- Ctypes fundamental data types: https://docs.python.org/2/library/ctypes.html#ctypes-fundamental-data-types-2
- Ctypes structures: https://docs.python.org/2/library/ctypes.html#structures-and-unions
- Sockets: https://docs.python.org/2/howto/sockets.html
"""

import socket
import sys
import random
import time
import signal
from ctypes import *


""" This class defines a C-like struct """
MapType = c_uint16*35*50
SnakeType = c_uint16*2*300
class Payload(Structure):
    _fields_ = [("snake_len", c_uint16),
                ("fx", c_uint16),
                ("fy", c_uint16),
                ("pos", SnakeType),
                ("map", MapType)
               ]

class Reply(Structure):
    _fields_ = [("direction", c_uint16)
               ]

class TimeoutError(Exception):
  pass

def timer_handler(signum, frame):
  raise TimeoutError("Timeout")

def get_direction(payload_out, host='localhost', port=2300, maxtime=1):
    signal.signal(signal.SIGALRM, timer_handler)
    signal.alarm(maxtime)
    try:
      server_addr = (host, port)
      s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      if s < 0:
          print "Error creating socket"
  
      try:
          s.connect(server_addr)
          #print "Connected to %s" % repr(server_addr)
      except:
          print "ERROR: Connection to %s refused" % repr(server_addr)
          raise RuntimeError("Connection to %s refused" % repr(server_addr))
  
      try:
          for i in range(5):
              print ""
              nsent = s.send(payload_out)
              # Alternative: s.sendall(...): coontinues to send data until either
              # all data has been sent or an error occurs. No return value.
              #print "Sent %d bytes" % nsent
  
              buff = s.recv(sizeof(Reply))
              d = Reply.from_buffer_copy(buff).direction
              print "Player",host,":",port,"sent direction:",d
              signal.alarm(0)
              return d
              '''print "Received id=%d, counter=%d, temp=%f" % (payload_in.id,
                                                          payload_in.counter,
                                                          payload_in.temp)'''
      finally:
          #print "Closing socket"
          s.close()
    except TimeoutError, exc:
      return None
  

if __name__ == "__main__":
    main()
