
# Low cost air quality IoT
This report gives the details of the sensor network and technical requirements.

## Primary Device: [Seeeduino LoRaWAN with GPS](https://m.seeedstudio.com/productDetail/2781/)
- Raspberry Pi 3 Model B
- [RHF0M301 module](http://www.risinghf.com/#/product-details?product_id=6&lang=en/)

### Sensors
These sensors are attached to the device.

| Sensor name     | Description  | Interface  |
|---------------------|-----------------|------------|
| [Grove Dust Sensor](https://www.seeedstudio.com/Grove-Dust-Sensor-p-1050.html/)  | Measures low pulse occupancy time as a correlate to particular matter. | Digital pin |
| [Grove Barometer Sensor](https://www.seeedstudio.com/depot/Grove-Barometer-Sensor-BMP280-p-2652.html/) | Measures temperature (celcius) and atmospheric pressure. | I2C | 
| [Grove Multichannel Gas Sensor](https://www.seeedstudio.com/Grove-Multichannel-Gas-Sensor-p-2502.html/) | Measures carbon monoxide, nitrogen dioxide, ethanol, hydrogen, ammonia, methane, propane, and iso-butane in ppm. | I2C |

### Physical Characteristics
#### Dimensions
Seeeduino device dimensions with add-ons: 65mm x 55mm x 45mm (L x W x H) <br/>
Gateway device dimensions: 85mm x 55mm x 30mm (L x W x H, without antenna)

Seeeduino device enclosure:
- See "3DModel" folder for computerized mode of the casing. <br/>
Gateway device enclosure: (7.00" H x 3.73" W x 3.98" D)

#### Weight
Seeeduino device weight with add-ons: 0.25 kilograms <br/>
Gateway device weight: X

Seeeduino device with enclosure: X  </br>
Gateway device with enclosure: X

#### Power Requirements

Average power consumption on standby: 1.1 watt <br/>
Average power consumtpion during transmission: 1.2 watt <br/>
Voltage usage: 4.91 volts <br/>
Current usage: 0.20 amps

- [3.7 V Lithium Ion Battery](https://www.amazon.com/Battery-Lithium-2000mAh-Compatible-Controller/dp/B0137ITW46)
- [3W SEEED Solar Panel](https://cpc.farnell.com/seeed-studio/313070001/solar-panel-138x160-3w/dp/MK00376)

