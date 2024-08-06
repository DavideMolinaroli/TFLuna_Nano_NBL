import serial

# Define the serial port and baud rate 
ser = serial.Serial('COM15', 230400) 
file_path = "output.txt"
samples = 0

try: 
    with open(file_path, 'a') as file:
        while samples <= 267: 
            if ser.in_waiting > 0:
                # Read a line from the serial port 
                line = ser.readline().decode('utf-8').strip() 
                
                # Print the line read from the serial port 
                print(line)
                file.write(line + '\n') 
                samples += 1
    
except KeyboardInterrupt: 
    # Close the serial port when the program is interrupted 
    ser.close() 