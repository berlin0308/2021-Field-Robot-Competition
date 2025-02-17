# 2021 NTU BME Field Robot Competition


## Features

- NVIDIA Jetson TX2
- Arduino Uno / Mega 2560
- TX2-Arduino transmission via serial: /dev/ttyACM0
- Mechanical Arm used to grab/drop objects
- Lane tracking algorithm using opencv image processing
- Fruit, Carrot, colored Box detection
- Monitoring the Depth and the Temperature
- Climbing on slanted synthetic grass surface 

## File Structure

+ /Main - Python, Arduino main programs
+ /Arduino - Arduino code
+ /Arrow_detection - detecting Arrow's direction, distance with opencv
+ /Lane_detection - finding path on synthetic grass with opencv
+ /Fruit_detection - identifying Fruits, grab/drop them with the Arm
+ /Carrot_finder - recognizing the Carrot instead of white radishes
+ /Tests - camera, LED, serial, Arm, server motor, DHT, urltrasonic, IR sensor
+ /Tools - useful tools for image processing 
