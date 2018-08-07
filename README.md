Excavator simulator
=====

It is an excavator simulator integrated with the game `DIG IT! - A Digger Simulator`, but can be easily integrated with another game.

[Link to the video](https://youtu.be/R2LVpZUrTIc)

The code was created for the needs of my eight-year-old son who is a huge fan of construction machinery.

The whole architecture was based on the Arduino Pro Micro microcontroller, which has a USB 2.0 interface thanks to which the driver can be seen as a USB device. In my case, this allowed me to simulate pressing a combination of keys on the keyboard.

The simulator has been equipped with: four analog joysticks that realistically reflect the physics of machine motion,
ignition switch to run the board, 2x16 LCD display with I2C interfac, which in the first assumption was supposed to show speed and basic parameters, but the game manufacturer did not foresee the possibility of writing modifications. The simulator has also been equipped with six Arcade buttons with backlight to control additional functions, eg turning on the radio, lights, etc. Arduino does not have enough pins, so the simulator has two PCF8574 integrated circuits, thanks to which we obtained an additional 16 digital pins.

![Sketch](https://raw.github.com/mserafin/Excavator-Simulator/master/sketch.png)
