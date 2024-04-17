#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>



#define JS_EVENT_BUTTON     0x01    /* button pressed/released */
#define JS_EVENT_AXIS       0x02    /* joystick moved */
#define JS_EVENT_INIT       0x80    /* initial state of device */


void starting_msg(){
    printf("[*] C script to use game controller to activate push to talk\n");
    printf("[*] Written by v01d.dev\n");
    printf("[*] Always use X to exit\n");
}

void emit(int fo, int type, int key, int value){
    struct input_event event;
    gettimeofday(&event.time, NULL);
    event.type = type;
    event.code = key;
    event.value = value;
    write(fo, &event, sizeof(event));
}


int main(){
    struct js_event {
        __u32 time;     /* event timestamp in milliseconds */
        __s16 value;    /* value */
        __u8 type;      /* event type */
        __u8 number;    /* axis/button number */
    };

    starting_msg();

    sleep(1);

    int fd = open("/dev/input/js0", O_RDONLY);
    struct js_event e;

    int of = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    struct uinput_setup usetup;

    if (of < 0){
        printf("Unable to open /dev/uinput\n");
        return -1;
    }

    // configuring uinput device
    ioctl(of, UI_SET_EVBIT, EV_KEY);
    ioctl(of, UI_SET_KEYBIT, KEY_RIGHTCTRL); // change keys to what is needed

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1;
    usetup.id.product = 0x1;
    strcpy(usetup.name, "Virtual Keyboard");

    ioctl(of, UI_DEV_SETUP, &usetup);
    ioctl(of, UI_DEV_CREATE);

    int JoyPressed = 0;

    // using X button to break

    while(1){
        read(fd, &e, sizeof(e));

        /*
        // Useful output to be used when testing
        printf("----------\n");
        printf("value --> %d\ntype --> %d\nnumber --> %d\n", e.value, e.type, e.number);
        printf("----------\n");
        */

        int ButtonState = e.value;

        if (JoyPressed == 1){
            emit(of, EV_KEY, KEY_RIGHTCTRL, 1);
            emit(of, EV_SYN, SYN_REPORT, 0);
        }

        if (e.value == 1) {
            if (e.number != 2){
             JoyPressed = 1;
            }
            else{
                break;
            }
        } 

        else if (e.value == 0){
            emit(of, EV_KEY, KEY_RIGHTCTRL, 0);
            emit(of, EV_SYN, SYN_REPORT, 0);
        }
    }

    // Destroy uinput device
    ioctl(of, UI_DEV_DESTROY);
    close(of);
    return 0;
}
