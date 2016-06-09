%{
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
%}

% ArCOM uses an Arduino library to simplify serial communication of different
% data types between MATLAB/GNU Octave and Arduino. To use the library,
% include the following at the top of your Arduino sketch:
% #include "ArCOM.h". See documentation for more Arduino-side tips.
%
% Initialization syntax:
% serialObj = ArCOM('open', 'COM3')
% where 'COM3' is the name of Arduino's serial port on your system.
% This call both creates and opens the port. It returns a struct containing
% a serial port and properties. If PsychToolbox IOport interface is
% available, this is used by default. To use the java interface on a system
% with PsychToolbox, use ArCOM('open', 'COM3', 'java')
%
% Write: ArCOM('write', serialObj, myData, 'uint8') % where 'uint8' is a
% data type from the following list: 'uint8', 'uint16', 'uint32'. If no
% data type argument is specified, ArCOM assumes uint8.
%
% Read: myData = ArCOM('read' serialObj, nValues, 'uint8') % where nValues is the number
% of values to read, and 'uint8' is a data type from the following list: 'uint8', 'uint16', 'uint32'
% If no data type argument is specified, ArCOM assumes uint8.
%
% End: ArCOM('close', serialObj) % Closes, deletes and clears the serial port
% object in the workspace of the calling function

function varargout = ArCOM(op, varargin)
switch lower(op)
    case 'open'
        arCOMObject = struct;
        arCOMObject.Port = [];
        if (exist('OCTAVE_VERSION'))
            try
                pkg load instrument-control
            catch
                error('Please install the instrument control toolbox first. See http://wiki.octave.org/Instrument_control_package');
            end
            if (exist('serial') ~= 3)
                error('Serial port communication is necessary for Pulse Pal, but is not supported in Octave on your platform.');
            end
            warning('off', 'Octave:num-to-str');
            arCOMObject.UseOctave = 1;
            arCOMObject.Interface = 2; % Octave serial interface
        else
            arCOMObject.UseOctave = 0;
        end
        try
            V = PsychtoolboxVersion;
            arCOMObject.UsePsychToolbox = 1;
            arCOMObject.Interface = 1; % PsychToolbox serial interface
        catch
            arCOMObject.UsePsychToolbox = 0;
            arCOMObject.Interface = 0; % Java serial interface
        end
        portString = varargin{1};
        if nargin > 2
            baudRate = varargin{2};
            if isstr(baudRate)
                baudRate = str2double(baudRate);
            end
        else
            error('Error: Please add a baudRate argument when calling ArCOM(''open''')
        end
        if ~isnan(baudRate) && baudRate >= 1200
            arCOMObject.baudRate = baudRate;
        else
            error(['Error: ' baudRate ' is an invalid baud rate for ArCOM. Some common baud rates are: 9600, 115200'])
        end
        if nargin > 3
            forceOption = varargin{2};
            switch lower(forceOption)
                case 'java'
                    arCOMObject.UsePsychToolbox = 0;
                    arCOMObject.Interface = 0;
                case 'psychtoolbox'
                    arCOMObject.UsePsychToolbox = 1;
                    arCOMObject.Interface = 1;
                otherwise
                    error('The third argument to ArCOM(''init'' must be either ''java'' or ''psychtoolbox''');
            end
        end
        arCOMObject.validDataTypes = {'char', 'uint8', 'uint16', 'uint32', 'int8', 'int16', 'int32'};
        switch arCOMObject.Interface
            case 0
                arCOMObject.Port = serial(portString, 'BaudRate', 115200, 'Timeout', 1,'OutputBufferSize', 100000, 'InputBufferSize', 100000, 'DataTerminalReady', 'on', 'tag', 'ArCOM');
                fopen(arCOMObject.Port);
            case 1
                if ispc
                    portString = ['\\.\' portString];
                end
                IOPort('Verbosity', 0);
                arCOMObject.Port = IOPort('OpenSerialPort', portString, 'BaudRate=115200, OutputBufferSize=100000, DTR=1');
                pause(.1); % Helps on some platforms
                varargout{1} = arCOMObject;
            case 2
                if ispc
                    PortNum = str2double(portString(4:end));
                    if PortNum > 9
                        portString = ['\\\\.\\COM' num2str(PortNum)]; % As of Octave instrument control toolbox v0.2.2, ports higher than COM9 must use this syntax
                    end
                end
                arCOMObject.Port = serial(portString, 115200,  1);
                pause(.2);
                srl_flush(arCOMObject.Port);
        end
    case 'bytesavailable'
        arCOMObject = varargin{1};
        switch arCOMObject.Interface
            case 0 % MATLAB/Java
                varargout{1} = arCOMObject.Port.BytesAvailable;
            case 1 % MATLAB/PsychToolbox
                varargout{1} = IOPort('BytesAvailable', arCOMObject.Port);
            case 2 % Octave
                error('Reading available bytes from a serial port buffer is not supported in Octave as of instrument control toolbox 0.2.2');
        end
    case 'write'
        arCOMObject = varargin{1};
        data = varargin{2};
        if nargin > 3
            dataType = varargin{3};
            if ~strcmp(dataType, arCOMObject.validDataTypes)
                error(['The datatype ' dataType ' is not currently supported by ArCOM.']);
            end
        else
            dataType = 'uint8';
        end
        switch dataType % Check range
            case 'char'
                if sum((data < 0)+(data > 128)) > 0
                    error('Error: a char was out of range: 0 to 128 (limited by Arduino)')
                end
            case 'uint8'
                if sum((data < 0)+(data > 255)) > 0
                    error('Error: an unsigned 8-bit integer was out of range: 0 to 255')
                end
            case 'uint16'
                if sum((data < 0)+(data > 65535)) > 0
                    error('Error: an unsigned 16-bit integer was out of range: 0 to 65,535')
                end
            case 'uint32'
                if sum((data < 0)+(data > 4294967295)) > 0
                    error('Error: an unsigned 32-bit integer was out of range: 0 to 4,294,967,295')
                end
            case 'int8'
                if sum((data < -128)+(data > 127)) > 0
                    error('Error: a signed 8-bit integer was out of range: -128 to 127')
                end
            case 'int16'
                if sum((data < -32768)+(data > 32767)) > 0
                    error('Error: a signed 16-bit integer was out of range: -32,768 to 32,767')
                end
            case 'int32'
                if sum((data < -2147483648)+(data > 2147483647)) > 0
                    error('Error: a signed 32-bit integer was out of range: -2,147,483,648 to 2,147,483,647')
                end
        end
        switch arCOMObject.Interface
            case 0
                fwrite(port, data, dataType);
            case 1
                switch dataType
                    case 'char'
                        IOPort('Write', arCOMObject.Port, char(data), 1);
                    case 'uint8'
                        IOPort('Write', arCOMObject.Port, uint8(data), 1);
                    case 'uint16'
                        IOPort('Write', arCOMObject.Port, typecast(uint16(data), 'uint8'), 1);
                    case 'uint32'
                        IOPort('Write', arCOMObject.Port, typecast(uint32(data), 'uint8'), 1);
                    case 'int8'
                        IOPort('Write', arCOMObject.Port, typecast(int8(data), 'uint8'), 1);
                    case 'int16'
                        IOPort('Write', arCOMObject.Port, typecast(int16(data), 'uint8'), 1);
                    case 'int32'
                        IOPort('Write', arCOMObject.Port, typecast(int32(data), 'uint8'), 1);
                end
            case 2
                switch Datatype
                    case 'char'
                        srl_write(arCOMObject.Port, char(data));
                    case 'uint8'
                        srl_write(arCOMObject.Port, char(data));
                    case 'uint16'
                        srl_write(arCOMObject.Port, char(typecast(uint16(data), 'uint8')));
                    case 'uint32'
                        srl_write(arCOMObject.Port, char(typecast(uint32(data), 'uint8')));
                    case 'int8'
                        srl_write(arCOMObject.Port, char(typecast(int8(data), 'uint8')));
                    case 'int16'
                        srl_write(arCOMObject.Port, char(typecast(int16(data), 'uint8')));
                    case 'int32'
                        srl_write(arCOMObject.Port, char(typecast(int32(data), 'uint8')));
                end
        end
        
    case 'read'
        arCOMObject = varargin{1};
        nValues = varargin{2};
        if nargin > 3
            dataType = varargin{3};
            if ~strcmp(dataType, arCOMObject.validDataTypes)
                error(['The datatype ' dataType ' is not currently supported by ArCOM.']);
            end
        else
            dataType = 'uint8';
        end
        switch arCOMObject.Interface
            case 0
                varargout{1} = fread(port, nValues, dataType);
            case 1
                nValues = double(nValues);
                switch dataType
                    case 'char'
                        varargout{1} = char(IOPort('Read', arCOMObject.Port, 1, nValues));
                    case 'uint8'
                        varargout{1} = uint8(IOPort('Read', arCOMObject.Port, 1, nValues));
                    case 'uint16'
                        Data = IOPort('Read', arCOMObject.Port, 1, nValues*2);
                        varargout{1} = typecast(uint8(Data), 'uint16');
                    case 'uint32'
                        Data = IOPort('Read', arCOMObject.Port, 1, nValues*4);
                        varargout{1} = typecast(uint8(Data), 'uint32');
                    case 'int8'
                        varargout{1} = typecast(uint8(IOPort('Read', arCOMObject.Port, 1, nValues)), 'int8');
                    case 'int16'
                        Data = IOPort('Read', arCOMObject.Port, 1, nValues*2);
                        varargout{1} = typecast(uint8(Data), 'int16');
                    case 'int32'
                        Data = IOPort('Read', arCOMObject.Port, 1, nValues*4);
                        varargout{1} = typecast(uint8(Data), 'int32');
                end
            case 2
                nValues = double(nValues);
                switch Datatype
                    case 'char'
                        Data = srl_read(arCOMObject.Port, nValues);
                        varargout{1} = char(typecast(Data, 'int8'));
                    case 'uint8'
                        varargout{1} = srl_read(arCOMObject.Port, nValues);
                    case 'uint16'
                        Data = srl_read(arCOMObject.Port, nValues*2);
                        varargout{1} = typecast(Data, 'uint16');
                    case 'uint32'
                        Data = srl_read(arCOMObject.Port, nValues*4);
                        varargout{1} = typecast(Data, 'uint32');
                    case 'int8'
                        Data = srl_read(arCOMObject.Port, nValues);
                        varargout{1} = typecast(Data, 'int8');
                    case 'int16'
                        Data = srl_read(arCOMObject.Port, nValues*2);
                        varargout{1} = typecast(Data, 'int16');
                    case 'int32'
                        Data = srl_read(arCOMObject.Port, nValues*4);
                        varargout{1} = typecast(Data, 'int32');
                end
        end
    case 'close'
        arCOMObject = varargin{1};
        switch arCOMObject.Interface
            case 0
                fclose(arCOMObject.Port);
                delete(arCOMObject.Port);
            case 1
                IOPort('Close', arCOMObject.Port);
            case 2
                fclose(arCOMObject.Port);
                arCOMObject.Port = [];
        end
        evalin('caller', ['clear ' inputname(2)])
    otherwise
        error('Error: call to ArCOM with an invalid op argument. Valid arguments are: open, bytesAvailable, read, write, close')
end