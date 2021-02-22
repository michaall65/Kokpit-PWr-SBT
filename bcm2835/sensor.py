#import w1thermsensor
from time import sleep

#sensor = w1thermsensor.W1ThermSensor()
counter = 0

while True:
    #temp = sensor.get_temperature()
    #temp = repr(temp)
    counter += 1
    plik = open("/home/pi/Desktop/ePaper-Project/bcm2835/obj/data.txt","w")
    if plik.writable():
       plik.write(repr(counter))
       plik.close()
    print(counter)
    sleep(0.5)

