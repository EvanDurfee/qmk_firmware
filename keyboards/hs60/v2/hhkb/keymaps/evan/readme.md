The default keymap for HHKB HS60 V2. VIA support disabled.
==========================================================

![Layout image](https://imgur.com/usbrQWL.png)

Default layer is normal HHKB with 7U space. Fn layer is used for RGB functions, Volume control and arrow cluster

Installation:
Use dfu-util --list to show devices, and add args to the command:

dfu-util -D file.bin -R -d 0483:df11 -a 0 -s 0x08000000:leave

Or use the command to build and flash qmk in one step. E.g.:

make hs60/v2/hhkb:evan:dfu-util

Reset:
Unplug -> Hold escape -> Plug in -> Unplug -> Release escape -> Plug in

Flash:
Unplug -> Hold escape -> plug in -> flash
