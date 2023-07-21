#!/bin/sh

# mount configfs
mount -t configfs none /sys/kernel/config

modprobe libcomposite
CONFIGFS_ROOT=/sys/kernel/config
#mount none $CONFIGFS_ROOT -t configfs

#1. Creating the gadgets

mkdir $CONFIGFS_ROOT/usb_gadget/g1
cd $CONFIGFS_ROOT/usb_gadget/g1

#Each gadget needs to have its vendor id <VID> and product id <PID> specified:
# USB ids
echo 0x1d6b > idVendor
echo 0x104 > idProduct

# USB strings, optional
mkdir strings/0x409 # US English, others rarely seen
echo "RioTboard" > strings/0x409/manufacturer
echo "ECM" > strings/0x409/product

#2. Creating the configurations
# create the (only) configuration
mkdir configs/c.1 # dot and number mandatory

# ensure function is loaded
modprobe usb_f_acm

#3. Creating the functions
# create the (only) function mkdir functions/ecm.usb0 # .
mkdir functions/ecm.0
#4. Associating the functions with their configurations
# assign function to configuration ln -s functions/ecm.usb0/ configs/c.1/
ln -s functions/ecm.0/ configs/c.1/
#5. Enabling the gadget
# bind!
echo ci_hdrc.0 > UDC # ls /sys/class/udc to see available UDCs

#ifconfig usb0 192.168.9.2 up
ip a add 192.168.9.2/255.255.255.0 dev usb0
ip link set dev usb0 up
