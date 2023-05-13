# How to

We already have the module we need to load (from assignment01), so, to load it
when a USB keyboard is plugged in, we will need to use a `udev` rule, we have two possible approachs:

## Approachs

### USB Specification

The `ATTR{bInterfaceClass}` and `ATTR{bInterfaceProtocol}` approach is
considered more specific and possibly more accurate because these attributes are
part of the USB specification and correspond to specific types of devices.

The `bInterfaceClass` and `bInterfaceProtocol` attributes are assigned by the
device manufacturer and should accurately describe the device's function
according to the USB standard.

- `bInterfaceClass` describes the general type of device. For example, `03`
is the class code for Human Interface Devices (HID), which includes keyboards,
mice, game controllers, etc.
- `bInterfaceProtocol` is a further specification within that class. For HID
devices, `01` corresponds to a keyboard.

Using these attributes, you can identify the exact type of device you're dealing
with, down to the specific class and protocol within the USB specification.
This allows you to target very specific types of devices in your udev rules.

```udev
# /etc/udev/rules.d/99-my_module.rules

ACTION=="add", SUBSYSTEM=="usb", ATTR{bInterfaceClass}=="03", ATTR{bInterfaceProtocol}=="01", RUN+="/sbin/modprobe my_module"
```

### Linux Kernel Input Subsystem

On the other hand, `ENV{ID_INPUT_KEYBOARD}=="1"` is a higher-level, more generic
property that's set by the input subsystem of the Linux kernel.

It doesn't provide as much specificity as the USB attributes, and there might be
edge cases where a device doesn't correctly identify itself to the input
subsystem, leading to inaccuracies.

```udev
# /etc/udev/rules.d/99-my_module.rules

ACTION=="add", SUBSYSTEM=="usb", ENV{ID_INPUT_KEYBOARD}=="1", RUN+="/sbin/modprobe my_module"
```

### Conclusion

So, if you know for sure that you're dealing with standard USB keyboards and you
want to specifically target these devices, the `bInterfaceClass` and
`bInterfaceProtocol` approach might be more accurate. However, the difference is
likely to be negligible in most cases, and the simpler
`ENV{ID_INPUT_KEYBOARD}=="1"` approach should work fine for most USB keyboards.

## Creating a udev rule

There's no need to go low-level when the kernel provide us higher-level stuff,
so I will use the Input Subsystem approach.

But, for some reason this approach does not work with an Apple Magic Keyboard,
and since it is the one I will use to test this, I'm forced to take the
secand and low-level approach.

First I create the udev rule:
```udev
# /etc/udev/rules.d/99-hello_keyboard.rules

#ACTION=="add", SUBSYSTEM=="usb", ENV{ID_INPUT_KEYBOARD}=="1", RUN+="/usr/sbin/insmod /home/dpoveda/git/little-penguin-1/assignment04/main.ko"

ACTION=="add", SUBSYSTEM=="usb", ATTR{bInterfaceClass}=="03", ATTR{bInterfaceProtocol}=="01", RUN+="/usr/sbin/insmod /home/dpoveda/git/little-penguin-1/assignment04/main.ko"
```

## Reload rules

And now reload the udev rules (as `root` !):
```bash
udevadm control --reload-rules
```

## Test it !

I just connect and disconect the keyboard of the neighbor mac, hehe :3
