import serial
import getpass
import time

print("Please swipe your card now: ")
serialPort = '/dev/ttyACM0'
#serialPort = '/dev/ttyACM1'
baudRate = 9600
ser = serial.Serial(serialPort, baudRate)
time.sleep(0.5)
while True:
	n = ser.read();
	if n == 0:
		pass
        elif n == 'R':
		break

OS = input("Please Choose your OS: 1. Linux 2. Windows 3. Mac : ")
password = getpass.getpass("Enter your password: ")
serialData = ""
serialData += str(OS) 
serialData += password 
serialData += " "
ser.write(serialData)
