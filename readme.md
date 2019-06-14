# ESP32 Temp AIO

Based around the [Heltec WiFi Kit32](https://www.amazon.com/Development-0-96inch-Display-Arduino-Compatible/dp/B07428W8H3) with integrated OLED display, and external DS18B20 temperature sensors (2).

Logs two temperatures to Adafruit IO. I based the AIO side of this on their [temperature and humidity tutorial](https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity/arduino-code), with "humidity" simply renamed to "temperature 2" and so on. How it all works should be pretty straightforward after looking at that tutorial.

## Configuration
The stock Heltec board is connected to the two DS18B20s with GND, 3.3V, and data to Pin 5. Code will probably get angry if not exactly two are present, and which is "T1" vs "T2" is arbitrary based on internal ID.

## Motivation
I built this to log hot tub water and air temperature to Adafruit IO, for science. I wanted to measure heating rate, cooling rate, and energy cost, and this was the fastest way to put together a live-logging 2-channel temperature measurement. 