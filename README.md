# Wemos_d1_gasmesser

This repository makes use of a QMC5883L Magnetic-Field Sensor rather then a reed-contact to detect changes in your gas consumption (Gasverbrauch).
Basicly inspired was this by this Rasperry-Pi Project.

[Gaszähler auslesen mit Magnetometer HMC5883 und Raspberry Pi](https://www.kompf.de/tech/gascountmag.html)

Required Material:
 - Wemos D1 Mini
 - QMC5883L Sensor - i did use this one Youmile GY-273 QMC5883L - Alternative you can use a HMC5883 - but you need then to use another library.
 
 Architecture:
  Gas counter --> Sensor --> Wemos D1 Wifi connections --> MQTT Server
  
Available values:
- Magnetic Field / Calculated as |b| = SQRT(X² + Y² + Z²)
- x-axis
- y-axis
- z-axis
- step-value

Background
----------

The gas meter has a magnet on the wheel - which you can count when it reaches a position again. This results in a change of the magnetic field.
