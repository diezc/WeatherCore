WeatherSparkCore
================

A simple project using multiple sensor to create a Weather Station powered by the Spark Core. The core reads temperature values from different sensors and calculates the average. 
It also measures barometric preassure, air humidity and light levels in the room.
All of these are posted to the Spark Cloud, so they could be accessed easily. 

This is one of my very firsts Spark Core projects and usage of GitHub to post my code.

Sensors used:
-TMP36: simple analog temperature sensor
-DHT11: Temperature & Humidity sensor with a One.Wire interface.
-BMP180: Barometric and Temperature sensor with a i2c interface.
-LDR: a simple light depending resistor.

The project makes use of two libraries, adapted to be used with the Spark Core:
-"ADAFRUIT_BMP085"
-"ADAFRUIT_DHT"

By @diezc
