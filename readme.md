# IoT Particulate Sensors

## Introduction 
With the increase in particulate emissions over the years, there is a need to be aware of the implications of toxic airbourne particles. These can have negative health outcomes on the population. Currently, air quality sensors are spaced far and wide from eachother collecting data at large time frames. They can also be often expensive. This project seeks to solve these issues and presents a low cost sensor network using a novel network communication known as LoRaWAN, a set of arduinos, and a gateway linux based Raspberry Pi 3. Combined with the calibration technique using machine learning algorithms, this sensor network can effectively provide the public with an accurate representation of air quality.

## Specifications
Uses a set of 10 arduino based nodes and 1 gateway Raspberry Pi 3.

- [Seeeduino LoRaWAN with GPS](https://m.seeedstudio.com/productDetail/2781/)
- [Grove Dust Sensor](https://www.seeedstudio.com/Grove-Dust-Sensor-p-1050.html/) 
- [Grove Barometer Sensor](https://www.seeedstudio.com/depot/Grove-Barometer-Sensor-BMP280-p-2652.html/) : I2C protocol

- Raspberry Pi 3 Model B
- [RHF0M301 module](http://www.risinghf.com/#/product-details?product_id=6&lang=en/) : serves as the communication bridge for the Raspberry Pi 3.
