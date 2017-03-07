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

cd "$(dirname "$0")"

set -e

make clean
make
rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions modules.order Module.symvers

echo "Build done."

