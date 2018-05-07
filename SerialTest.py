import serial

ser = serial.Serial('COM8', 9600)
while True:
    msg = input("enter msg")
    ser.write(bytes(msg, "utf-8"))