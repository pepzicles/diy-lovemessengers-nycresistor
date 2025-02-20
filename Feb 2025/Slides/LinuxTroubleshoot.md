# Arduino IDE not detecting port

Many Linux distros come with a BRLTTY application that allows people hard-of-sight to access the computer's text-based console through a braille display. These displays use a serial port to communicate with the computer, but it takes over all general purpose USB ports. This causes the port to not appear in the Arduino IDE ports list. You can read more [**here**](https://github.com/arduino/help-center-content/issues/155).

You can simply uninstall BRLTTY, but if people find the application useful, this [**link**](https://unix.stackexchange.com/questions/670636/unable-to-use-usb-dongle-based-on-usb-serial-converter-chip/670637#670637) shares a more in-depth approach.

```
sudo apt-get remove brltty
```
I restarted my computer after doing this and the Ports link in Arduino was no longer greyed out. 

# Arduino IDE not given permission for port access to compile

A temporary testing solution is to give read, write, and execute access to the port with this command:

```
sudo chmod a+rw /dev/ttyUSB0 
```

However a more permanent solution is to add yourself to the dialout group for full and direct access to serial ports. You can read more [**here**](https://support.arduino.cc/hc/en-us/articles/360016495679-Fix-port-access-on-Linux#:~:text=rules%20on%20Linux.-,Add%20yourself%20to%20the%20dialout%20group,communicate%20on%20the%20serial%20ports.).






