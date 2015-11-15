# Pololu autonomous drive


## What you will need
* Pololu 3pi
* Some cables
* 1 SRF05 ultrasound sensor


## The set up
The setup is pretty simple. We just need to connect the SRF05 sensor to the proper pins in the pololu 3pi.
VCC and GND pins are straightforward. Second, we use the one-single-ping configuration of the SRF05 so we send the ping and receive the echo using the same pin. This ping goes to PD0 in the Pololu 3pi.

Finally, we ground the echo pin.

That's all!

You can see it in action ![here](https://youtu.be/w6TYFMHX_Kw)

Here is a picture of the connections.

![schematics](https://github.com/fernape/pololu/blob/master/ultra-sonic-sensor/pololu_ulstrasound_schematics.png)
![real connection](https://github.com/fernape/pololu/blob/master/ultra-sonic-sensor/shot.png)
