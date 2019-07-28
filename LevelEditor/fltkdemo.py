#!/usr/bin/python3

import sdl2
from sdl2 import *

import fltk
from fltk import *
import sys

def main():
    #create_gui()

    window = Fl_Window(300,180)
    box = Fl_Box(20,40,260,100,"Hello, World!")
    box.box(FL_UP_BOX)
    box.labelsize(36)
    box.labelfont(FL_BOLD+FL_ITALIC)
    box.labeltype(FL_SHADOW_LABEL)
    window.end()
    window.show(sys.argv)
    Fl.run()

if __name__ == '__main__':
    main()

