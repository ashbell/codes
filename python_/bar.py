#! /usr/bin/python

x = 10
def foo():
    y = 5
    bar = lambda y =y : x + y
    print bar()
    y = 8
    #bar = lambda y = y: x + y
    print bar()

foo()


