from tkinter import *


root = Tk()

root.counter = 0

labels = []

def grid_update():
    root.counter += 1
    label(root.counter)


def label(row_number):
    l1 = Label(root, borderwidth=1, relief = "solid", height=2, width=15, text = "1")
    l2 = Label(root, borderwidth=1, relief = "solid", height=2, width=15, text = "2")

    l1.grid(row = row_number, column = 0)
    l2.grid(row = row_number, column = 1)

    labels.append((l1, l2))

def wipe():
    for l1, l2 in labels:
        l1.destroy()
        l2.destroy()
    labels.clear()
    

l1 = Label(root, borderwidth=1, relief = "solid", height=2, width=15, text = "1")
l2 = Label(root, borderwidth=1, relief = "solid", height=2, width=15, text = "2")

l1.grid(row = 0, column = 0, padx = 10)
l2.grid(row = 0, column = 1, padx = 10)

set = Button(root, height=2, width=5, text = "set", command = grid_update)
reset = Button(root, height=2, width=5, text = "reset", command = wipe)

set.grid(row = 0, column = 2, padx = 10)
reset.grid(row = 0, column = 3, padx = 10)


root.mainloop()