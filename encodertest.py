import serial
import time

def calculate_checksum(data):
    return sum(data) & 0xFF

def reset_encoder(serial_port):
    try:
        # Open the serial port
        ser = serial.Serial(serial_port, baudrate=19200, timeout=1)

        # Set the encoder address
        encoder_address = '01'  # Assuming the encoder address is 01

        # Send the reset command (example command format)
        reset_command = f'D{encoder_address}LM{calculate_checksum([0x44, 0x30, 0x31, 0x4C, 0x4D])}\r'.encode('ascii')
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

def read_encoder_angle(serial_port):
    try:
        # Open the serial port
        ser = serial.Serial(serial_port, baudrate=19200, timeout=1)

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

if __name__ == "__main__":
    serial_port = 'COM9'
    previous_angle = None
    revolution = 0

    # Reset the encoder to start from zero
    reset_encoder(serial_port)

    while True:
        # Read the current angle
        current_angle = read_encoder_angle(serial_port)
        
        if current_angle is not None:
            if previous_angle is not None:
                # Check for clockwise or counterclockwise rotation across the 360/0 boundary
                if previous_angle > current_angle and (previous_angle - current_angle) > 180:
                    revolution += 1
                    print(f"Completed one clockwise revolution. Total revolutions: {revolution}")
                elif current_angle > previous_angle and (current_angle - previous_angle) > 180:
                    revolution -= 1
                    print(f"Completed one counterclockwise revolution. Total revolutions: {revolution}")

                # Calculate the absolute angle considering the number of revolutions
                absolute_angle = revolution * 360 + current_angle

                # Display previous and current angles, along with the absolute angle
                print(f"Previous angle: {previous_angle} degrees")
                print(f"Current angle: {current_angle} degrees")
                print(f"Absolute angle: {absolute_angle} degrees")
            else:
                print(f"Current angle: {current_angle} degrees (Initial reading)")
            
            # Update the previous angle
            previous_angle = current_angle
        
        # Delay for 1 second before the next reading
        time.sleep(1)
