#!/bin/bash
#
# This file is part of tty0tty project and software.
# 
# Copyright (c) : 2013  Luis Claudio Gambôa Lopes. All rights reserved.
#
# The tty0tty software is DUAL licensed. It is made available under the terms of the GNU Affero 
# General Public License (AGPL) v3.0 for non-commercial use and under the terms of a commercial 
# license for commercial use of this software. 
#
# The tty0tty software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#################################################################################################

# Run this script as root user. This script has been tested with Ubuntu 12.04.

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root user !" 1>&2
   exit 0
fi

cd "$(dirname "$0")"

KDIR=$(uname -r)

dfile="./tty0tty.ko"
if [ -f "$dfile" ]
then
    cp ./tty0tty.ko /lib/modules/$KDIR/kernel/drivers/misc
    echo "resolving dependencies..."
    depmod
else
    echo "driver file tty0tty.ko not found in current directory !"
    exit 0
fi

ufile="./99-tty0tty.rules"
if [ -f "$ufile" ]
then
    cp ./99-tty0tty.rules /etc/udev/rules.d/
    udevadm control --reload-rules
    udevadm trigger --attr-match=subsystem=tty
else
    echo "udev rule file 99-tty0tty.rules not found in current directory !"
    exit 0
fi

echo tty0tty >> /etc/modules
echo "installation complete !"
