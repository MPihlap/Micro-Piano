import keyboard
import serial
import time

pressed_keys = []


def cb(kbevent):
    if not kbevent.name in pressed_keys:
        pressed_keys.append(kbevent.name)
        print(kbevent.name + " has been pressed")
        # print(kbevent.scan_code)


def init_uart(baud=9600, stopBit=1, parity=0):
    ser = serial.Serial('COM8', baud, timeout=0.001)
    return ser


keyboard.on_press(cb)
ser = init_uart()
# Protocol: word amount and then data
while True:
    msg = "0"
    for i in pressed_keys:
        if not keyboard.is_pressed(i):
            print(i + " is not pressed anymore")
            pressed_keys.pop(pressed_keys.index(i))
            msg = "0"
            ser.write(bytes(msg, "utf-8"))
        else:
            msg = i
            ser.write(bytes(msg, "utf-8"))
    # msg = str(len(pressed_keys)) + msg
    #ser.write(bytes(msg, "utf-8"))
    if keyboard.is_pressed('esc'):
        break
    time.sleep(0.01)
print("Exiting program")
ser.close()
