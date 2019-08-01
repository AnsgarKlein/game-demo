#!/usr/bin/env python3

import os

import ctypes
from ctypes import *

import sdl2
from sdl2 import *

import tkinter as tk
from tkinter import *

global SDL_RENDERER
SDL_RENDERER = None

global SDL_WINDOW
SDL_WINDOW = None

class VerticalScrolledFrame(Frame):
    def __init__(self, parent, *args, **kw):
        Frame.__init__(self, parent, *args, **kw)

        # Create canvas object with two scrollbars
        v_scroll = Scrollbar(self, orient=VERTICAL)
        v_scroll.pack(fill=Y, side=RIGHT, expand=FALSE)

        h_scroll = Scrollbar(self, orient=HORIZONTAL)
        h_scroll.pack(fill=X, side=BOTTOM, expand=FALSE)

        canvas = Canvas(self, bd=0, highlightthickness=0,
                        yscrollcommand=v_scroll.set,
                        xscrollcommand=h_scroll.set)
        canvas.pack(side=LEFT, fill=BOTH, expand=TRUE)

        v_scroll.config(command=canvas.yview)
        h_scroll.config(command=canvas.xview)

        # Reset the canvas view
        canvas.xview_moveto(0)
        canvas.yview_moveto(0)

        # Create frame inside canvas which will be scrolled
        self.interior = interior = Frame(canvas)
        interior_id = canvas.create_window(0, 0, window=interior, anchor=NW)

        # Track changes to canvas and inner frame width and also update scrollbar
        def _configure_interior(event):
            # Update scrollbars to match size of inner frame
            size = (interior.winfo_reqwidth(), interior.winfo_reqheight())
            canvas.config(scrollregion="0 0 %s %s" % size)
            if interior.winfo_reqwidth() != canvas.winfo_width():
                # Update canvas width to fit inner frame
                canvas.config(width=interior.winfo_reqwidth())

        def _configure_canvas(event):
            if interior.winfo_reqwidth() != canvas.winfo_width():
                # Update inner frame width to fill canvas
                canvas.itemconfigure(interior_id, width=canvas.winfo_width())

        interior.bind('<Configure>', _configure_interior)
        canvas.bind('<Configure>', _configure_canvas)

def render():
    print('render()')
    root.update()

    global SDL_RENDERER
    global SDL_WINDOW

    SDL_SetRenderDrawColor(SDL_RENDERER,
                           ctypes.c_ubyte(255),
                           ctypes.c_ubyte(0),
                           ctypes.c_ubyte(255),
                           ctypes.c_ubyte(255))

    SDL_RenderClear(SDL_RENDERER)

    SDL_RenderPresent(SDL_RENDERER)

def create_gui():
    global root
    root = Tk()

    # Scrolled frame
    scroll_frame = VerticalScrolledFrame(root)
    scroll_frame.pack()
    label = Label(root, text="Resize window")
    label.pack()

    # Buttons (1)
    for i in range(3):
        b = Button(scroll_frame.interior, text='Button ' + str(i), command = render)
        b.pack(side=TOP)

    # embed frame
    embed_frame = Frame(scroll_frame.interior, width = 400, height = 400)
    embed_frame.pack(side=TOP)

    # Buttons (2)
    button_frame = Frame(scroll_frame.interior)
    for i in range(10):
        b = Button(button_frame, text='BUTTON ' +str(i), command = render)
        b.pack(side=RIGHT)
    button_frame.pack(side=TOP)

    # Buttons (3)
    for i in range(3):
        b = Button(scroll_frame.interior, text='Button ' +str(i), command = render)
        b.pack(side=TOP)

    # Update tk for the first time
    root.update()

    # SDL2
    SDL_Init(SDL_INIT_VIDEO)
    global SDL_WINDOW
    SDL_WINDOW = SDL_CreateWindowFrom(embed_frame.winfo_id())

    #global SDL_RENDERER
    global SDL_RENDERER
    SDL_RENDERER = SDL_CreateRenderer(SDL_WINDOW, -1, 0)


def main():
    create_gui()

    #while True:
    #    render()

    root.after(0, render)
    root.mainloop()

if __name__ == "__main__":
    main()
    sys.exit(0)

