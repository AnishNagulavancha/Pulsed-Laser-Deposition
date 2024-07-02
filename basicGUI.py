from tkinter import *
import datetime

root = Tk()

root.geometry('760x760')

root.counter = 0

def clicked():
    root.counter += 1
    if root.counter % 2:
        state.config(text = "saving")
    else:
        state.config(text = "idle")
    
display = Label(root, height = 2, width = 10, text = "0", fg="white", bg = "black")
display.pack(side=LEFT, padx=10, pady=10)

state = Label(root, height = 2, width = 10, text = "0", fg="white", bg = "black")
state.pack()

button = Button(root, text="save", command = lambda:clicked())
button.pack(side=LEFT, padx=10, pady=10)

current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
str_current_datetime = str(current_datetime)
file_name = str_current_datetime+".txt"

with open(file_name, 'w') as file:
    file.write("")

def time_update():
    current_datetime = datetime.datetime.now().strftime("%m-%d %H-%M-%S")
    str_current_datetime = str(current_datetime)
    with open(file_name, 'a') as file:
        file.write(str_current_datetime + "\n")
    root.after(1000, time_update)


root.after(1000, time_update)

root.mainloop()
