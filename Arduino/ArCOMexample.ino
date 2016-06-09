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

// This example code receives 100 16-bit signed integers and 100 32-bit unsigned integers, modifies them, and returns them to MATLAB.
// On the MATLAB side, use: 
// Port = ArCOM('open', 'COM13', 115200)
// ArCOM('write', Port, int16(sin(.1*(1:100))*10000), 'int16'); % Write a 16-bit sine wave
// ArCOM('write', Port, 100001:100100, 'uint32'); % Write 32-bit timestamps
// ArCOM('read', Port, 100, 'int16'); % Read 100 16-bit samples
// ArCOM('read', Port, 100, 'uint32'); % Read 100 32-bit samples
// ArCOM('close', Port);

#include "ArCOM.h"
ArCOM myUSB;
short waveform[100] = {0};
unsigned long timestamps[100] = {0};

void setup() {
  SerialUSB.begin(115200);
}

void loop() {
  if (SerialUSB.available()) { // Wait for MATLAB to send data
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
