import sys
import glob
import serial
from serial.tools import list_ports
import time
import ctypes
import numpy as np
import struct


"""
This function scans for available ports and returns a reference to the first
one that it is able to successfully open a serial connection with
"""
def connectArduino():
   #find out which ports are available
   ports = serial.tools.list_ports.comports()
   if not ports: # make sure we have a valid port to connect to
      print "Unable to find a valid connection, please check connection and rerun the script"
      return -1

   # iterate over the ports until we are able to connect to one of them
   for com in ports:
      port = serial.Serial(com[0], baudrate=115200,timeout=3.0)
      if port:
         return port

def packInt16(value):
   return struct.pack( "<H",value)

def sendData(channels, port):
   message = '{'
   for channel in channels:
      message +=  str(unichr(channel/100))
      message +=  str(unichr(channel%100))
      # checksum to be implemented later
   message += '}'
   port.write(message)
   print message

def sendTest(num,port):
   channels = np.array([1*num, 2*num, 3*num, 4*num ,5*num, 6*num, 7*num, 8*num])
   sendData(channels,port)