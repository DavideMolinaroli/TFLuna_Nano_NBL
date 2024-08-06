import math
from collections import deque

import matplotlib.animation as animation
import matplotlib.pyplot as plt
import serial

ser = serial.Serial('COM15', 230400) 

data_x = deque([0]*267, maxlen = 267)
data_y = deque([0]*267, maxlen = 267)

# Create a figure and axis for plotting
fig, ax = plt.subplots()
line = ax.scatter([], [], s=5, color='b')

def read_data():
    if ser.in_waiting > 0:
        try:
            rline = ser.readline().decode('utf-8').strip() 
            print(rline)
            
            tokens = rline.split()
            dist_cm = float(tokens[0]) # in cm
            angle_rad = (float(tokens[1])%360) * math.pi/180 # in rad

            data_x.append(dist_cm*math.cos(angle_rad))
            data_y.append(dist_cm*math.sin(angle_rad))
        except: 
            pass   
        # 267 Measurements make up one full scan

# Animation function
def animate(frame):
    read_data()
    line.set_offsets(list(zip(data_x, data_y)))
    return line

# Set labels
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_xlim(-800, 800)
ax.set_ylim(-800, 800)

# Add a legend to the plot
ax.legend()

# Create an animation object
ani = animation.FuncAnimation(fig, animate, interval=10)

# Show the plot
plt.show()

# Close the serial port when done
ser.close()