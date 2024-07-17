import minimalmodbus
from tkinter import *
import datetime

class TempGUI():
    def __init__(self, num_controller, com, ports):
        self.num_controller = num_controller
        self.com = com
        self.ports = ports
        self.labels = []
        self.instruments = []

        self.root = Tk()
        self.root.geometry('760x760')

        for port in self.ports:
            instrument = minimalmodbus.Instrument(com, port)
            instrument.serial.baudrate = 9600
            instrument.close_port_after_each_call = True
            self.instruments.append(instrument)

        main_frame = Frame(self.root)
        main_frame.pack(pady=50)

        label_frame = Frame(main_frame)
        label_frame.pack(side=LEFT)

        for i in range(0, num_controller):
            display = Label(label_frame, height=2, width=15, text=str(0), fg="white", bg="black")
            display.pack(pady=5)
            self.labels.append(display)

        self.button = Button(main_frame, text="save")
        self.button.pack(side=LEFT, padx=10)
        
        self.current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
        self.file_name = self.current_datetime + ".txt"

        with open(self.file_name, 'w') as file:
            file.write("      Date       " + "  ".join([f"Temperature {i+1}" for i in range(num_controller)]) + "  \n")
        
        self.temp_update()

        self.root.mainloop()

    def read_temperature(self, instrument):
        try:
            temperature = instrument.read_register(4096, 1)  # Register number, number of decimals
            print(temperature)
            return temperature
        except IOError:
            print("Failed to read from instrument")
            return None

    def temp_update(self):
        temperatures = []
        for i, instrument in enumerate(self.instruments):
            temp = self.read_temperature(instrument)
            if temp is not None:
                self.labels[i].config(text=str(temp))
                temperatures.append(temp)

        current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
        with open(self.file_name, 'a') as file:
            file.write(" " + current_datetime + "  " + "  ".join([str(temp) for temp in temperatures]) + " \n")

        self.root.after(4000, self.temp_update)