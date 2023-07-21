#!/bin/bash

#Each gadget needs to have its vendor id <VID> and product id <PID> specified:
# USB ids
cd /sys/kernel/config/usb_gadget/g1
echo "0x1d6b" > idVendor 
echo "0x104" > idProduct 

# USB strings, optional
cd /sys/kernel/config/usb_gadget/g1/strings/0x409
echo "RioTboard" > manufacturer
echo "ECM" > product

#5. Enabling the gadget
# bind!
cd /sys/kernel/config/usb_gadget/g1
echo "ci_hdrc.0" > UDC # ls /sys/class/udc to see available UDCs

#ifconfig usb0 192.168.9.2 up
ip a add 192.168.9.2/255.255.255.0 dev usb0

ip link set dev usb0 up

exit 0
