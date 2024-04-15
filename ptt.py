#!/bin/python
import os
import pyautogui
from inputs import get_gamepad


def main():

    while 1:

        events = get_gamepad()
        for event in events:

            button = event.code
            state = event.state
            
            if button != "SYN_REPORT" and "ABS" not in button:

                print(button)
                if state == 1:
                    print("[*] Key down detected, turning on")
                    pyautogui.keyDown('ctrlright')
                else:
                    print("[*] Key up detected, turning off")
                    pyautogui.keyUp('ctrlright')


if __name__ == "__main__":
    main()
