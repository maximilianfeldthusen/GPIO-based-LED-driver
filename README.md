## Documentation 

### GPIO-based-LED-driver

This code sets up a simple **GPIO-based LED driver** in the Linux kernel, and it's split into multiple sections:

### 1) **Device Tree Source (DTS)**
The DTS file defines the LED in a **Device Tree**, which is used to describe hardware to the Linux kernel:
- `/dts-v1/;` declares the DTS format.
- The `leds` node describes a **GPIO LED**, compatible with `"mycompany,gpio-led"`.
- The `led@0` node defines an LED connected to **GPIO pin 17**, which is **active high**.
- The `label` describes how it appears in `/sys/class/leds/`.

### 2) **The Driver (gpio_led.c)**
The **C driver** implements the LED control:
- It defines a `gpio_led` structure to manage the LED.
- The `gpio_led_set` function sets the brightness (0 or 1) by toggling the **GPIO pin**.
- The `gpio_led_probe` function:
  - Allocates driver memory.
  - Gets the LED's GPIO descriptor.
  - Registers the LED under `/sys/class/leds/`.
- The driver is registered in `gpio_led_driver`, matching `"mycompany,gpio-led"` from the DTS.

### 3) **Makefile**
The Makefile handles **kernel module compilation**, ensuring `gpio_led.o` is compiled.

### 4) **Build & Test**
- **Compile** the module using `make`.
- **Insert** the module using `insmod gpio_led.ko`.
- **Check** `/sys/class/leds/` to see the registered LED.
- **Control** the LED by writing `0` or `1` to `/sys/class/leds/mycompany:green:myled/brightness`.
- **Blink** the LED using a **timer** trigger.



 
