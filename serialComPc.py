import sys
import glob
import serial
import time
import ctypes
import numpy as np
import struct
import wmi
from codecs import encode, decode

"""
This function scans for available ports and returns a reference to the first
one that it is able to successfully open a serial connection with
"""
def connectArduino():
   #find out which ports are available

   query = "SELECT * FROM Win32_PnPEntity WHERE Name LIKE '%(COM%)'"
   coms  = wmi.WMI().query(query)

   arduino = 0
   for com in coms:
      print(com.Name)
      if com.Name.find("Arduino",0) > -1:
         arduino = (com.Name[com.Name.find("(",0)+1:com.Name.find(")",0)])
         print (arduino)
         break
   
   if arduino == 0: # make sure we have a valid port to connect to
      print ("Unable to find a valid connection, please check connection and rerun the script")
      return -1

   # iterate over the ports until we are able to connect to one of them
   port = serial.Serial(arduino, baudrate=115200,timeout=3.0)
   if port:
      return port
   else:
      print("Unable to connect to the Arduino at the specified baud rate")
      return -1


def packInt16(value):
   return struct.pack( "<H",value)

def sendData(channels, port):
   message = '{'
   for channel in channels:
      channel = int(channel)
      print(channel)
      message += channel.to_bytes(2, byteorder='big')
      # checksum to be implemented later
   message += '}'
   port.write(message)

def sendTest(num,port):
   channels = np.array([1*num, 2*num, 3*num, 4*num ,5*num, 6*num, 7*num, 8*num])
   sendData(channels,port)

def sendInt16(number,port):
   number = int(number).to_bytes(2, byteorder='big')
   
   port.write(number)


