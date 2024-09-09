# -*- coding: utf-8 -*-
"""
Created on Tue Aug 27 15:32:17 2024

@author: zekai, Anish
"""

## 
# Created on Tue Aug 27 15:32:17 2024
# @Author: Zekai.Yao, Anish.Nagulavancha
##

import serial
import time
from tkinter import *

class EncoderGUI:
    def __init__(self, serial_port):
        self.serial_port = serial_port
        self.previous_angle = 0
        self.revolution = 0

        # Set up the main window
        self.root = Tk()
        self.root.geometry('400x400')

        # Create a frame for the main content
        main_frame = Frame(self.root)
        main_frame.pack(pady=20)

        # Create labels for displaying angle and revolution data
        self.angle_label = Label(main_frame, height=2, width=30, text="Current angle: ", fg="black", bg="white")
        self.angle_label.pack(pady=5)

        self.revolution_label = Label(main_frame, height=2, width=30, text="Total revolutions: 0", fg="black", bg="white")
        self.revolution_label.pack(pady=5)

        self.absolute_angle_label = Label(main_frame, height=2, width=30, text="Absolute angle: ", fg="black", bg="white")
        self.absolute_angle_label.pack(pady=5)

        # Create a button to reset the encoder
        self.button = Button(main_frame, text="Reset Encoder", command=self.reset_encoder)
        self.button.pack(pady=10)

        # Start updating the angle display
        self.update_angle()

        # Start the GUI main loop
        self.root.mainloop()

    def calculate_checksum(self, data):
        return sum(data) & 0xFF

    def reset_encoder(self):
        try:
            # Open the serial port
            ser = serial.Serial(self.serial_port, baudrate=19200, timeout=1)

            # Set the encoder address
            encoder_address = '01'  # Assuming the encoder address is 01

            # Send the reset command (example command format)
            reset_command = f'D{encoder_address}LM{self.calculate_checksum([0x44, 0x30, 0x31, 0x4C, 0x4D])}\r'.encode('ascii')
            ser.write(reset_command)

            # Receive the reset response
            response = ser.read(16)

            # Verify if the reset was successful
            if len(response) == 16 and response[5:7] == b'lm':
                print("Encoder has been reset to zero.")
            else:
                raise Exception("Failed to reset the encoder")

        except Exception as e:
            print(f"Error resetting encoder: {e}")
        finally:
            ser.close()

        # Reset internal state
        self.previous_angle = 0
        self.revolution = 0
        self.revolution_label.config(text="Total revolutions: 0")

    def read_encoder_angle(self):
        try:
            # Open the serial port
            ser = serial.Serial(self.serial_port, baudrate=19200, timeout=1)

            # Set the encoder address
            encoder_address = '01'  # Assuming the encoder address is 01

            # Send the read command
            command = f'D{encoder_address}\r'.encode('ascii')
            ser.write(command)

            # Receive the response data
            response = ser.read(16)

            # Check if the response data length is correct
            if len(response) == 16:
                # Extract the angle data part
                angle_data = response[5:15].decode('ascii').strip()
                print(angle_data)
                angle = int(angle_data) % 4095   # Wrap the angle within the 0-360 range
                return angle
            else:
                raise Exception("Incorrect response data length")
        
        except Exception as e:
            print(f"Error reading angle: {e}")
            return None
        finally:
            ser.close()

    def update_angle(self):
        current_angle = (360 / 4096)*self.read_encoder_angle() 

        if current_angle is not None:
            # If current angle is less than the previous angle, it indicates a full revolution
            if current_angle < self.previous_angle:
                self.revolution += 1  # Increase revolution count on wrap-around

            # Calculate absolute angle as revolutions * 360 + current angle
            absolute_angle = self.revolution * 4096 + current_angle

            # Update labels with the current angle and revolution count
            self.absolute_angle_label.config(text=f"Absolute angle: {absolute_angle} degrees")
            self.angle_label.config(text=f"Current angle: {current_angle} degrees")
            self.revolution_label.config(text=f"Total revolutions: {self.revolution}")

            # Update previous angle for the next iteration
            self.previous_angle = current_angle

        # Schedule the next update
        self.root.after(100, self.update_angle)

if __name__ == "__main__":
    serial_port = 'COM5'  # Replace with your actual serial port
    gui = EncoderGUI(serial_port)