<h1>Advanced Projects</h1>
<h2>Schematic Design -- Status Update</h2>
<p>We are currently at Checkpoint 3 -- communication between two boards using the RF modules through structs.
  If you look at the code we just uploaded, we called all the function required to initialize the RF24 object in setup().
  The code is similar for both the Arduino and the Teensy. The Teensy is responsible to listen for anything that is transmitted by the Arduino.
  <a>http://tmrh20.github.io/RF24/classRF24.html</a> This website contains basic documentation of all the functions in the RF24 library.
</p>

