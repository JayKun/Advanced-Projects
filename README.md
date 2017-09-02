<h1>Advanced Projects</h1>
<h2>Air Mouse</h2>
<p>
 Air Mouse is a gyroscopic PC mouse built from scratch. 
 The Air Mouse was developed incrementally as there were several key technologies that needed to be explored and verified.
</p>

<h2>Stage 1 - Serial Communication</h2>
<h3>Wack-A-Mole</h3>
In this lab, utilizng both an Arduino Pro Mini and Teensy LC,  we implemented a simple game that utilizes the serial communication.
The Teensy LC is tasked to record button presses while the Arduino receives the button presses and verifies the pattern.

<h2>Stage 2  - SPI Communication</h2>
In this lab, we implemented the same game but both boards are communicating wirelessly or more specifically, using radiowaves. The RF24 module is connected to the board via SPI. 

<h2>Stage 3 - I2C Communication</h2>
In this lab, we started incorporating the gyrometer into the design of our Air Mouse. The IMU sensor is connected to the Arduino via I2C communication.
We combine all 3 communication implementations to create a fucntional prototype of the Air Mouse. The final code is located in the Lab3 directory. 

<h2>Future Innovations</h2>
Program more customized shortcuts using specific hand gestures. 
