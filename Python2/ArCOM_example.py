'''
----------------------------------------------------------------------------

This file is part of the Sanworks ArCOM repository
Copyright (C) 2016 Sanworks LLC, Sound Beach, New York, USA

----------------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3.

This program is distributed  WITHOUT ANY WARRANTY and without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''
# To use this code, load the example Arduino sketch in /ArCOM/Arduino
# Update the serial port string below ('COM3') to your actual port.
from ArCOM import ArCOMObject # Import ArCOMObject
serialPort = ArCOMObject('COM3', 115200) # Create a new instance of an ArCOM serial port
serialPort.write([5]*100, 'uint16', [500]*100, 'uint32')
myVoltages, myTimes = serialPort.read(100, 'uint16', 100, 'uint32')
print myVoltages
print myTimes
