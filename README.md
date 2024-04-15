# gamepad_ptt
A program that allows me to use my Bluetooth controller to activate push to talk on Linux. It was developed for the Logitech F710 and only tested on Xorg Arch. A solution was implemented in both python and C.

### C Code
Using linux/input.h to receive input from the gamepad and the linux/uinput.h to send input at the kernel level by creating a virtual device. Use the X button on the F710 to exit the program, this insures that the virtual device is killed and dealt with.

### Python Code
Using the inputs module to handle listening to the controller input and Pyautogui to handle sending keyboard input. There isn't much more to it, just pure function.
