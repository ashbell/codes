#! /usr/bin/python

from functools import partial
import Tkinter

root = Tkinter.Tk()
MyButon = partial (Tkinter.Button, root, fg = 'white', bg = 'blue')
b1 = MyButon (text = 'one')
b1.pack ()

root.title ('fpas')
root.mainloop()


