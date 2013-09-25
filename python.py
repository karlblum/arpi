import serial
ser = serial.Serial('COM3',9600)

command = 'wwwsssaaaddd'
ser.write(command.encode())
