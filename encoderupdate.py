import serial
import tkinter as tk

def read_serial_and_display():
    with serial.Serial('COM7', 115200, timeout=1) as ser:
        # Write the command to the serial port
        string = 'E1\r\n'
        ser.write(string.encode())
        
        # Read the response from the serial port
        line = ser.readline()
        
        # Extract the binary part from the line (removing 'E' prefix and '\r\n')
        binary_str = line.decode().strip()[1:]  # Remove 'E' and the newline characters
        
        # Convert the binary string to an integer
        binary_int = int(binary_str, 2)
        
        # Convert the integer to a hexadecimal string
        hex_str = hex(binary_int)

        # Update the labels with the binary and hexadecimal values
        binary_label.config(text=f"Binary: {binary_str}")
        hex_label.config(text=f"Hexadecimal: {hex_str}")

# Create the main window
root = tk.Tk()

# Create and place labels for Binary and Hexadecimal values
binary_label = tk.Label(root, text="Binary: ", height=2, width=25, fg="white", bg="black")
binary_label.pack(pady=10)

hex_label = tk.Label(root, text="Hexadecimal: ", height=2, width=25, fg="white", bg="black")
hex_label.pack(pady=10)

# Create a button to trigger the serial read function
read_button = tk.Button(root, text="Read from Serial", command=read_serial_and_display)
read_button.pack(pady=10)

# Run the Tkinter event loop
root.mainloop()
