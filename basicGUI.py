from tkinter import *
import datetime
import subprocess

root = Tk()

root.geometry('760x760')
root.counter = 0


def clicked():
    root.counter += 50
    value.config(text= str(root.counter))
    with open("time.txt", 'a') as file:
        file.write(str(datetime.datetime.now()))
        file.write("\n")
    
value = Label(root, height = 2, width = 10, text = "0", fg="white", bg = "black")
value.pack(side=LEFT, padx=10, pady=10)

button = Button(root, text="+50", command = lambda:clicked())
button.pack(side=LEFT, padx=10, pady=10)


#root.after(1000, open_time)

root.mainloop()
