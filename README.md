# Installation onto the system
1. Download the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install and run the Arduino IDE.
3. Click `Tools`, `Manage Libraries...`, and search for `MLX90614` and install `Adafruit MLX90614 Library` (not the `MiniMLX90614`!).
4. Click `Tools`, hover over `Board: (...)` and click `Boards Manager` (at the top). Search for `Arduino AVR Boards` and install it.
5. Click `Tools`, hover over `Board: (...)` and click (in the list) `Adafruit Circuit Playground`.
6. Connect the system to your computer with a micro-usb connector (not the USB-C connector, that can only charge the battery).
7. Click `Tools`, hover over `Port` and click the port that says `Adafruit Circuit Playground` next to `COM`.
8. Download the code in this repository by clicking [here](https://github.com/CocoTheOwner/Heat-Detecting-Wristband/archive/refs/heads/main.zip).
9. Unzip the `.zip` file into any folder (other than your Downloads folder).
10. Open the `main` folder, and open the `main.ino` file. (The Arduino IDE should now open).
11. Click `Upload` (the green, right-pointing arrow at the left top) to upload the program.

# Modifying the code
The parts you can easily modify are the top 20-odd lines (excluding the first and second line). 
They are configurable settings for the system.

You can also modify the rest of the code, but you must make sure you know what you're doing as to not compromise stability.

Enjoy!
