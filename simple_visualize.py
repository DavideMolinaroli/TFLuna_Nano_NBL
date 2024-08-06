import math

import matplotlib.pyplot as plt

# File path
file_path = 'output.txt'

# Lists to store the Cartesian coordinates
x_coords = []
y_coords = []

# Read data from the file
with open(file_path, 'r') as file:
    for line in file:
        try:
            # Parse the distance and angle from the line
            distance, angle = map(float, line.split())
            
            # Convert angle from degrees to radians
            angle_rad = math.radians(angle)
            
            # Convert polar coordinates to Cartesian coordinates
            x = distance * math.cos(angle_rad)
            y = distance * math.sin(angle_rad)
            
            # Append coordinates to the lists
            x_coords.append(x)
            y_coords.append(y)
        except ValueError:
            # Handle lines that don't contain valid float values
            print(f"Skipping invalid line: {line.strip()}")

# Create a scatter plot
plt.figure(figsize=(10, 6))
plt.scatter(x_coords, y_coords, c='blue', marker='o', edgecolor='k', s=50)
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.title('Scatter Plot of Cartesian Coordinates')
plt.grid(True)

# Show the plot
plt.show()