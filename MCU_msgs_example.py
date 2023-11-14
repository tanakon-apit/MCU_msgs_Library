import serial
import time
from MCU_msgs import mcu_msgs

esp_msgs = mcu_msgs(msg_size=3, header=71, carry_bit=2)
ser_msgs = mcu_msgs(msg_size=3, header=10, carry_bit=2)

ser = serial.Serial(port= "COM22", baudrate= 115200)
if not ser.is_open: ser.open()
while not ser.is_open:
    print("Serial not available!!!")

ser_msgs.assign_msgs(0, 12.34)
ser_msgs.assign_msgs(1, 5.87)
ser_msgs.assign_msgs(2, -8.9)

ser_time = time.time() + 0.01

while True:

    if (time.time() >= ser_time):
        ser_time = ser_time + 0.01
        ser.write(bytearray(ser_msgs.encode_msg2buffer()))

    if ser.in_waiting:
        val = int.from_bytes(ser.read(1), "big")
        esp_msgs.decode_buffer2msg(val=val)
        if esp_msgs.status:
            print(esp_msgs.get_msgs(0), esp_msgs.get_msgs(1), esp_msgs.get_msgs(2))
