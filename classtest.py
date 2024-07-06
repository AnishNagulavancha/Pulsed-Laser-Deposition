import minimalmodbus
from tkinter import *
import datetime


class temp_gui():
    def __init__(self, root):
        self.root = root
        self.state = Label(root, height=2, width=10, text="idle", fg="white", bg="black")
        self.state.pack(pady=10)

        self.instrument_1 = minimalmodbus.Instrument('COM5', 1)
        self.instrument_1.serial.baudrate = 9600
        self.instrument_1.close_port_after_each_call = True

        self.instrument_2 = minimalmodbus.Instrument('COM5', 5)
        self.instrument_2.serial.baudrate = 9600
        self.instrument_2.close_port_after_each_call = True

        main_frame = Frame(root)
        main_frame.pack(pady=50)

        label_frame = Frame(main_frame)
        label_frame.pack(side=LEFT)

        self.temp_display1 = Label(label_frame, height=2, width=15, text=str(0), fg="white", bg="black")
        self.temp_display1.pack(pady=5)

        self.temp_display2 = Label(label_frame, height=2, width=15, text=str(0), fg="white", bg="black")
        self.temp_display2.pack(pady=5)

        self.button = Button(main_frame, text="save", command=self.clicked)
        self.button.pack(side=LEFT, padx=10)

        self.root.geometry('760x760')
        self.root.counter = 0
        
        self.current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
        self.file_name = self.current_datetime + ".txt"

        with open(self.file_name, 'w') as file:
            file.write("|      Date      | Temperature 1 | Temperature 2 | \n")
        
        self.temp_update()
        

    def clicked(self):
        self.root.counter += 1
        if self.root.counter % 2:
            self.state.config(text="saving")
        else:
            self.state.config(text="idle")

    def read_temperature_1(self):
        try:
            temperature = self.instrument_1.read_register(4096, 1)  # Register number, number of decimals
            print(temperature)
            return temperature
        except IOError:
            print("Failed to read from instrument")

    def read_temperature_2(self):
        try:
            temperature = self.instrument_2.read_register(4096, 1)  # Register number, number of decimals
            print(temperature)
            return temperature
        except IOError:
            print("Failed to read from instrument")

    def time_update(self):
        current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
        str_current_datetime = str(current_datetime)
        temp1 = self.read_temperature_1()
        temp2 = self.read_temperature_2()
        self.temp_display1.config(text=str(temp1))
        self.temp_display2.config(text=str(temp2))
        with open(self.file_name, 'a') as file:
            file.write("| " + str_current_datetime + ", " + str(temp1) + " , " + str(temp2) + "\n")
        self.root.after(6000, self.time_update)


    def temp_update(self):
        temp1 = self.read_temperature_1()
        self.temp_display1.config(text=str(temp1))
        temp2 = self.read_temperature_2()
        self.temp_display2.config(text=str(temp2))
        self.root.after(4000, self.temp_update)

if __name__ == "__main__":
    root = Tk()
    app = temp_gui(root)
    root.mainloop()

#     def clicked(self, root, instrument_1, instrument_2):
#         root.counter += 1
#         if root.counter % 2:
#             state.config(text="saving")
#         else:
#             state.config(text="idle")

#     def read_temperature_1(self, instrument_2):
#         try:
#             temperature = instrument_2.read_register(4096, 1)
#             print(temperature)
#             return temperature
#         except IOError:
#             print("Failed to read from instrument")

#     def read_temperature_2(self,instrument_1):
#         try:
#             temperature = instrument_1.read_register(4096, 1)
#             print(temperature)
#             return temperature
#         except IOError:
#             print("Failed to read from instrument")

#     def time_update():
#         current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
#         str_current_datetime = str(current_datetime)
#         temp1 = read_temperature_1()
#         temp2 = read_temperature_2()

#         with open(file_name, 'a') as file:
#             file.write("| " + str_current_datetime + " |      " + str(temp1) + "     |      " + str(temp2) + "     |" + "\n")
#         root.after(4000, time_update)

#     temp1 = read_temperature_1()
#     temp2 = read_temperature_2()

#     root = Tk()
#     root.geometry('760x760')
#     root.counter = 0

#     main_frame = Frame(root)
#     main_frame.pack(pady=50)

#     label_frame = Frame(main_frame)
#     label_frame.pack(side=LEFT)

#     temp_display1 = Label(label_frame, height=2, width=15, text=str(temp1), fg="white", bg="black")
#     temp_display1.pack(pady=5)

#     temp_display2 = Label(label_frame, height=2, width=15, text=str(temp2), fg="white", bg="black")
#     temp_display2.pack(pady=5)
    
#     button = Button(main_frame, text="save", command=clicked)
#     button.pack(side=LEFT, padx=10)

#     state = Label(root, height=2, width=10, text="idle", fg="white", bg="black")
#     state.pack(pady=10)

#     current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
#     file_name = current_datetime + ".txt"

#     with open(file_name, 'w') as file:
#         file.write("|      Date      | Temperature 1 | Temperature 2 | \n")

#     def temp_update(self, root, temperature_1, temperature_2):
#         temp1 = read_temperature_1()
#         temp_display1.config(text=str(temp1))
#         temp2 = read_temperature_2()
#         temp_display2.config(text=str(temp2))
#         root.after(4000, temp_update)

# root.after(1000, time_update)
# root.after(4000, temp_update)
# root.mainloop()




# instrument_1 = minimalmodbus.Instrument('COM5', 1)  # port name, slave address (in decimal)
# instrument_1.serial.baudrate = 9600
# instrument_1.close_port_after_each_call = True

# instrument_2 = minimalmodbus.Instrument('COM5', 5)  # port name, slave address (in decimal)
# instrument_2.serial.baudrate = 9600
# instrument_2.close_port_after_each_call = True

