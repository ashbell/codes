#! /usr/bin/python 

import curses

screen = curses.initscr()

screen.border(0)
screen.addstr(12, 25, "Python curses in action!")
screen.refresh()
screen.getch()

curses.endwin()

