import math
import threading
import time
from collections import deque

import matplotlib.animation as animation
import matplotlib.pyplot as plt
import serial

# Replace 'COM3' with the appropriate COM port and adjust the baud rate as needed
SERIAL_PORT = 'COM15'
BAUD_RATE = 230400

# Number of samples to receive before updating the plot
UPDATE_INTERVAL = 267

# Initialize the serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Deques to store the data points for both dimensions
data_x = deque([], maxlen=UPDATE_INTERVAL)
data_y = deque([], maxlen=UPDATE_INTERVAL)

# Condition to signal when the data is ready for plotting
data_ready = threading.Event()

# Lock for synchronizing access to data_x and data_y
data_lock = threading.Lock()

# Event to signal threads to stop
shutdown_event = threading.Event()

def read_data():
    while not shutdown_event.is_set():
        if ser.in_waiting > 0:
            try:
                # Read a line of data from the serial port
                rline = ser.readline().decode('utf-8').strip()
                print(rline)
                # Split the line into two float values
                tokens = rline.split()
                dist_cm = float(tokens[0]) # in cm
                angle_rad = (float(tokens[1])%360) * math.pi/180 # in rad
                x = dist_cm*math.cos(angle_rad)
                y = dist_cm*math.sin(angle_rad)
                with data_lock:
                    # Append the values to the deques
                    data_x.append(x)
                    data_y.append(y)
                # Signal that the data is ready for plotting
                if len(data_x) >= UPDATE_INTERVAL:
                    data_ready.set()
            except Exception as e:
                print(f"Error: {e}")
        time.sleep(0.0001)  # Sleep briefly to prevent busy-waiting

def plot_data():
    fig, ax = plt.subplots()
    scat = ax.scatter([], [], s=5, color='b')  # Initialize an empty scatter plot with smaller markers
    
    def animate(frame):
        if data_ready.is_set():
            with data_lock:
                # Update scatter plot
                scat.set_offsets(list(zip(data_x, data_y)))
                # Reset the event for the next batch of data
                data_ready.clear()
        return scat,

    # Set labels
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_xlim(-800, 800)
    ax.set_ylim(-800, 800)

    # Create an animation object
    ani = animation.FuncAnimation(fig, animate, interval=100)

    # Show the plot
    plt.show()

# Start threads
data_thread = threading.Thread(target=read_data, daemon=True)
plot_thread = threading.Thread(target=plot_data, daemon=True)

data_thread.start()
plot_thread.start()

# Wait for the plot thread to finish
try:
    plot_thread.join()
finally:
    # Signal the data thread to stop
    shutdown_event.set()
    # Wait for the data thread to finish
    data_thread.join()
    # Close the serial port
    ser.close()