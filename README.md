RPI-5947-nixie-tubes
====================

This is a simple example of controlling Nixie tubes using the tcl5947 chips used in the http://cinchcircuits.com/ boards.  The boards take advantage of the 24 channels by using 0-11 for one Nixie and 12-23 for the other.  Using these boards, and the ability for the tlc5947 to daisy chain, you can have 16 boards, 32 nixie tubes.  But you will need to make sure enough current is supplied.  So you might need more than one high voltage supply.  Meaning you can onlt daisy chain the top HV boards 5-10 at a time.  But low voltage boards can just be daisy chained.
I have chronodot from http://http://macetech.com hooked in for a tie source.

The chronodot is a I2C device.  The raspberry pi OS you use does not always come ready for I2C. We need to do a bit of work to connect it.
 Adafruit has an excellent tutorial on installing it. But you first need to perform a few installation steps:
<ol>
<li> sudo apt-get libi2c-dev</li>
<li> modprobe i2c_dev, i2c_bcm2708, rtc_ds1307</li>
<li> lsmod top make sure they are all there.</li>
<li>  edit /etc/modprobe.d/raspi-blacklist.conf  and comment out the line: blacklist i2c-bcm2708</li>
<li> Edit /etc/modules and make sure that you add i2c_dev, i2c_bcm2708, rtc_ds1307 if they are not there.</li>
<li> reboot the Pi.</li>

Now you can follow the Adafruit tutortial.  http://learn.adafruit.com/adding-a-real-time-clock-to-raspberry-pi/wiring-the-rtc  But there are some notes.  The device will be /dev/rtc0 not /dev/rtc
Depending on when you run i2cdetect, the address might show up as 68, or as UU.  I do not know what the specific meanning of UU is, but it seems to mean it isbeing treated as the system RTC.  You can now just use the standard linux time library to access the RTC.  You do not need to make and i2c calls.


Now that we have a source of data, lets talk to those nixie tubes. You need to install the wiringPi and get the RasPiTLC5947 package.

TLC 5947 library: git://github.com/dgonyeo/RasPiTLC5947.git
wiringPi: https://projects.drogon.net/raspberry-pi/wiringpi/
The git repo for wiringPi is git://git.drogon.net/wiringPi
You will have a copy of the tlc lib in your project.  The header need to be modified, NUM_TLCS, to specify the number of boards you are daisy chaining.

The pin mapping mention tlc5947.c is the wiringPi package pin number.  So run "gpio readall" to see how the pins map. Depending on the diagram you find, you will use GPIO or name.  I used the adafruit breakout board, the pins names were the GPIO names.
<table>
<tr>
<th>tlc5947</th>
<th>Wiring Pin</th>
<th>GPIO</th>
<th>Name</th>
</tr>
<tr>
<td>SIN</td><td>0</td><td>17</td><td>GPIO 0</td>
</tr>
<tr><td>SCLK</td><td>4</td><td>23</td><td>GPIO 4</td></tr>
<tr><td>XLAT</td><td>2</td><td>27/21</td><td>GPIO 2</td></tr>
<tr><td>blank</td><td>3</td><td>23</td><td>GPIO 3</td></tr>
</table>

RasPiTLC5940
============

A small C program that can control the TLC5947 chip from TI with the GPIO pins on the Raspberry Pi

You need to have the WiringPi library installed. It's available at:
https://projects.drogon.net/raspberry-pi/wiringpi/

This program requires use of four of the GPIO pins on the Raspberry Pi. The required pins are specified in tlc5947.c

If you want to see this in use, check out my project ODD at:
https://github.com/dgonyeo/odd
