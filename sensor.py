import w1thermsensor
from time import sleep

sensor = w1thermsensor.W1ThermSensor()

while True:
    temp = sensor.get_temperature()
    temp = repr(temp)
    plik = open("/home/pi/Desktop/ePaper-Project/bcm2835/obj/data.txt","w")
    if plik.writable():
       plik.write(temp)
       plik.close()
    print(temp)
    sleep(10)

