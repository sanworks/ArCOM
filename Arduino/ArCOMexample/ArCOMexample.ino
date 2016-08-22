/*
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

*/

/* This example code receives 100 16-bit signed integers and 100 32-bit unsigned integers, modifies them, and returns them to MATLAB.
   In MATLAB, use: 
   Port = ArCOMObject('COM13', 115200);
   Port.write(sin(.1*(1:100))*10000), 'int16', 100001:100100, 'uint32'); % Write 16-bit sine wave, then 32-bit timestamps
   [wave, times] = Port.read(100, 'int16', 100, 'uint32'); % Read 100 signed 16-bit samples, then 200 unsigned 32-bit times
   clear Port; % clear the Object (releases the port)
   
   Or in Python, use:
   from ArCOM import ArCOMObject
   Port = ArCOMObject('COM13', 115200)
   Port.write([5]*100, 'uint16', [500]*100, 'uint32') % Write 16-bit sine wave, then 32-bit timestamps
   wave, times = Port.read(100, 'uint16', 100, 'uint32') % Read 100 signed 16-bit samples, then 200 unsigned 32-bit times
*/
#include "ArCOM.h"
ArCOM myUSB(SerialUSB); // Sets ArCOM to wrap SerialUSB (could also be Serial, Serial1, etc.)
short waveform[100] = {0};
unsigned long timestamps[100] = {0};

void setup() {
  SerialUSB.begin(115200);
}

void loop() {
  if (myUSB.available()) { // Wait for MATLAB to send data
    myUSB.readInt16Array(waveform, 100); // Read 100 16-bit signed integers (e.g. a waveform)
    myUSB.readUint32Array(timestamps, 100); // Read 100 32-bit unsigned integers (e.g. timing data)
    for (int i = 0; i < 100; i++) {
      waveform[i] = waveform[i]-1; // Modify each waveform sample
      timestamps[i] = timestamps[i]+1; // Modify each timestamp
    }
    myUSB.writeInt16Array(waveform, 100); // Write 100 16-bit signed integers
    myUSB.writeUint32Array(timestamps, 100); // Write 100 32-bit unsigned integers (e.g. timing data)
  }
}
