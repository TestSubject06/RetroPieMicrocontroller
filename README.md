# RetroPieMicrocontroller
This is the arduino sketch that runs the Feather microcontroller for an arcade cabinet project. Handles brownouts and power loss.

# Uploading this to the Microcontroller
First, this should only be necessary if the feather has just abruptly stopped working, and a press of the RESET button on the feather has no effect.

You'll need the latest version of the Arduino IDE. Then follow the setup guide here to install the extra boards manager: https://learn.adafruit.com/adafruit-feather-32u4-basic-proto/setup

Once that's done you can plug in the Feather, and select it from the Tools>Board... menu. Then make sure you select the correct port from the Tools>Port menu. The correct selection will look something like COMX(feather). Then you can click Upload on the sketch to send it up to the Feather. For more information about the setup you can view this page: https://learn.adafruit.com/adafruit-feather-32u4-basic-proto/using-with-arduino-ide
