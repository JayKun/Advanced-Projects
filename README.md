<h1>Advanced Projects</h1>
<h2>Air Mouse</h2>
<p>
 Air Mouse is a PC mouse that functions just like your ordinary PC mouse but can be used without operating it on a level surface. 
 Instead, the Air Mouse is controlled by a gyrometer on an IMU sensor (Inertial Measurement Unit). 
</p>

<h2>Lab1-Serial Communication</h2>
In this lab, we implemented a simple game that utilizes the Serial Communicationfeature on both an Arduino Pro Mini and a Teensy LC.
The Teensy LC is tasked to record button presses while the Arduino would recieve
the pattern of the button presses via serial communication.

<h2>Lab2-SPI Communication</h2>
In this lab, we implemented the same game but both boards are communicating wirelessly or more specifically, using a RF24 module. The module is connected to the board via SPI. 

<h2>Lab3-I2C Communication</h2>
In this lab, we start incorporating the gyrometer into the design of our Air Mouse. The IMU sensor is connected to the Arduino via I2C communication.
We combine all 3 communication implementations to create a fucntional prototype of the Air Mouse. The final code is located in Lab3. 
