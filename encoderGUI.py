import serial
import time
from tkinter import *
import datetime

class EncoderGUI:
    def __init__(self, serial_port):
        self.serial_port = serial_port
        self.previous_angle = None
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

            print(f"Raw response (bytes): {response}")

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
        self.previous_angle = None
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
                angle = int(angle_data)
                return angle
            else:
                raise Exception("Incorrect response data length")
        
        except Exception as e:
            print(f"Error reading angle: {e}")
            return None
        finally:
            ser.close()

    def update_angle(self):
        current_angle = self.read_encoder_angle()
        
        if current_angle is not None:
            if self.previous_angle is not None:
                if self.previous_angle > current_angle and (self.previous_angle - current_angle) > 180:
                    self.revolution += 1
                elif current_angle > self.previous_angle and (current_angle - self.previous_angle) > 180:
                    self.revolution -= 1

                absolute_angle = self.revolution * 360 + current_angle
                self.absolute_angle_label.config(text=f"Absolute angle: {absolute_angle} degrees")
            else:
                self.absolute_angle_label.config(text=f"Current angle: {current_angle} degrees (Initial reading)")
            
            self.angle_label.config(text=f"Current angle: {current_angle} degrees")
            self.revolution_label.config(text=f"Total revolutions: {self.revolution}")

            self.previous_angle = current_angle

        # Schedule the next update
        self.root.after(1000, self.update_angle)

if __name__ == "__main__":
    serial_port = 'COM9'  # Replace with your actual serial port
    gui = EncoderGUI(serial_port)
