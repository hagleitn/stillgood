# still good?

Ever wonder if that piece of fish in the fridge is still good? How long has it been in there? Well, build one of these and stick it on your leftovers. StillGood will keep count of the days for you. No more guessing.

<img src="https://github.com/hagleitn/stillgood/blob/master/afterlight.jpeg?raw=true" alt="Stillgood" width="500"/>

# Usage

If the display is off a single button press will bring it back to life. The display will turn off every 5 minutes to preserve power. Once activated the display will show the number of days since the last reset. The display only shows two digits, so it will wrap around every 100 days.

## Reset

When the display is active a push of the button will reset the number to "0". Count up will start again.

## Setting the number manually

A long press will enter into manual adjust mode. The number on the display starts blinking, which indicates the device is ready to receive clicks to increase the count. If no button presses are received for 5 seconds the device returns to normal operation.

## Changing the interval

By default the device counts up by one every day. However stillgood can also count every hour, minute or second. A double click on the button will cycle through the different modes. The display will indicate which mode is active after the change (d for day, h for hour, m for minute, s for second).

# Building one

Still good isn't hard to build but requires a fair amount of soldering because of the 2 digit 7 segment display. Here's the bill of materials

* Arduino pro mini, 3v 8Mhz 

<img src="https://github.com/hagleitn/stillgood/blob/master/arduino_pro_mini.jpg?raw=true" alt="arduino" width="150"/>

* 0.36", Common Anode, 2 digit, 7 segment, 10 pin display 

<img src="https://github.com/hagleitn/stillgood/blob/master/2_digit_7_segment_display.jpg?raw=true" alt="display" width="150"/>

* Push button, momentary switch (6x6mm)

<img src="https://github.com/hagleitn/stillgood/blob/master/button.jpg?raw=true" alt="button" width="150"/>

* 2 resistors (2k is a good starting point, depending on how bright you want the display)

<img src="https://github.com/hagleitn/stillgood/blob/master/resistor.gif?raw=true" alt="resistor" width="150"/>

* Battery, CR 2032

<img src="https://github.com/hagleitn/stillgood/blob/master/CR2032.jpg?raw=true" alt="battery" width="150"/>

* Wire to connect everything.

## Preparing the arduino

In order to reduce the power consumption of the arduino it's best to remove the voltage regulator and the led. In order to save space and avoid unwanted resets, I also remove the reset button. LED and button can be removed with a hobby knife. I remove the voltage regulator with needle nose pliers.

After prep, simply flash the binaries onto the arduino.

## Hooking things up

The display has to be wired to the pins of the arduino as follows:

    11 4 5 7 6
     9 x 3 8 10

The first row is for the top row of the pins of the display looking at it from the front. The second row is for the bottom pins. 2nd pin from left on the bottom row, does not have to be connected (controls the decimal point).

The resistors have to be placed between pins 10 and 11 on the arduino and the respective pins of the display.

One side of the button has to be connected to the pin 2 the other to ground on the arduino.

Finally, the battery needs to be connected to VCC and GND. I solder wires directly onto the battery after using some sandpaper on it (Careful not to heat the battery up too much).

The display and button should snap into place in the enclosure. Some hotglue helps to keep it in. The battery (after wrapping it in electric tape) goes below the display, on top of the button. The arduino on top of that. That's it.





