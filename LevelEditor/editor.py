#!/usr/bin/env python3

import os

import json

import random

import ctypes
from ctypes import *

import sdl2
from sdl2 import *
from sdl2.rect import *
from sdl2.rect import SDL_Rect

import sdl2.sdlimage
from sdl2.sdlimage import *

import tkinter as tk
from tkinter import ttk

LEVEL_DIR = 'levels/'
SPRITE_DIR = 'sprites/'

global SDL_RENDERER
SDL_RENDERER = None
global SDL_WINDOW
global SDL_EVENT
SDL_EVENT = SDL_Event()

global SPRITE_HANDLER
SPRITE_HANDLER = None

global LEVEL
LEVEL = None

global GRID_SIZE
GRID_SIZE = 32


def singleton(class_):
    instances = {}
    def getinstance(*args, **kwargs):
        if class_ not in instances:
            instances[class_] = class_(*args, **kwargs)
        return instances[class_]
    return getinstance

@singleton
class SpriteHandler:
    sprite_sheets = {}

    def __init__(self):
        for filename in os.listdir(SPRITE_DIR):
            if not filename.endswith('.json'):
                continue

            with open(SPRITE_DIR +filename) as json_data:
                data = json.load(json_data)
                sheet = SpriteSheet.from_json(data)
                sheet_id = sheet.sprite_id

                self.sprite_sheets[sheet_id] = sheet

    def get(self, sprite_id):
        return self.sprite_sheets[sprite_id]


class SpriteSheet:
    sprite_id = ""
    texture = None
    states = []

    def __init__(self, sprite_id, texture, states):
        self.sprite_id = sprite_id
        self.texture = texture
        self.states = states

    @staticmethod
    def from_json(json):
        JSON_KEY_ID = 'id'
        JSON_KEY_TEXTURE = 'texture'
        JSON_KEY_STATES = 'states'
        # TODO: parse states...

        required_keys = JSON_KEY_ID, JSON_KEY_TEXTURE, JSON_KEY_STATES

        # Check if all necessary keys are contained
        for key in required_keys:
            if key not in json:
                print('Error when parsing SpriteSheet.', file=sys.stderr)
                print('Did not find necessary key ' +key +'.', file=sys.stderr)
                return None

        # Extract values from json
        sprite_id = json[JSON_KEY_ID]
        texture_file = json[JSON_KEY_TEXTURE]

        # Load texture
        surf = IMG_Load(ctypes.c_char_p(texture_file.encode('utf-8')))
        if not surf:
            print('Error when loading SpriteSheet.', file=sys.stderr)
            print(SDL_GetError(), file=sys.stderr)
            return None

        global SDL_RENDERER
        texture = SDL_CreateTextureFromSurface(SDL_RENDERER, surf)
        if not texture:
            print('Error when loading SpriteSheet.', file=sys.stderr)
            print(SDL_GetError(), file=sys.stderr)
            return None

        return SpriteSheet(sprite_id, texture, None)

    def render(self, x, y, state = "default"):
        global GRID_SIZE
        global SDL_RENDERER

        clip = sdl2.rect.SDL_Rect(0, 0, GRID_SIZE, GRID_SIZE)

        # Set viewport to location where to render
        view = SDL_Rect(x, y, GRID_SIZE, GRID_SIZE)
        SDL_RenderSetViewport(SDL_RENDERER, view)

        # Render
        ret = SDL_RenderCopy(SDL_RENDERER, self.texture, clip, None);
        #print(str(ret))
        if ret != 0:
            print(type(self.texture))
            print(SDL_GetError())
            pass

class Level:
    level_id = ""
    width = 0
    height = 0
    background = []
    solid = []

    def __init__(self, level_id, width, height, background, solid):
        self.level_id = level_id
        self.width = int(width)
        self.height = int(height)
        self.background = background
        self.solid = solid

    @staticmethod
    def from_json(json):
        JSON_KEY_ID = 'id'
        JSON_KEY_WIDTH = 'width'
        JSON_KEY_HEIGHT = 'height'
        JSON_KEY_OBJECTS = 'objects'
        JSON_KEY_BACKGROUND = 'background'
        JSON_KEY_BACKGROUND_SPRITE = 'sprite'
        JSON_KEY_BACKGROUND_SOLID = 'solid'

        required_keys = JSON_KEY_ID, JSON_KEY_WIDTH, JSON_KEY_HEIGHT, \
                        JSON_KEY_OBJECTS, JSON_KEY_BACKGROUND

        # Check if all necessary keys are contained
        for key in required_keys:
            if key not in json:
                print('Error when parsing Level.', file=sys.stderr)
                print('Did not find necessary key ' +key +'.', file=sys.stderr)
                return None

        # Extract values from json
        parsed_id = json[JSON_KEY_ID]
        parsed_width = json[JSON_KEY_WIDTH]
        parsed_height = json[JSON_KEY_HEIGHT]
        parsed_size = parsed_width * parsed_height
        parsed_objects = json[JSON_KEY_OBJECTS]
        parsed_json_background = json[JSON_KEY_BACKGROUND]
        parsed_background_sprites = []
        parsed_background_solid = []

        # Check if there are exactly as many objects as announced
        if parsed_size != len(parsed_json_background):
            print('Error when parsing Level.', end='', file=sys.stderr)
            if parsed_size > len(parsed_json_background):
                print('Not enough values', end='', file=sys.stderr)
            else:
                print('Too many values', file=sys.stderr)
            print(' in ' +JSON_KEY_BACKGROUND +' array.', file=sys.stderr)
            print('Expected ' +parsed_size +' values.', file=sys.stderr)
            print('Found ' +len(parsed_json_background) +' values.', file=sys.stderr)
            return None

        # Go through all background objects
        for background_jobj in parsed_json_background:
            for key in JSON_KEY_BACKGROUND_SPRITE, JSON_KEY_BACKGROUND_SOLID:
                if key not in background_jobj:
                    print('Error when parsing background objects of Level.', file=sys.stderr)
                    print('Did not find necessary key' +key +'.', file=sys.stderr)
                    return None

            global SPRITE_HANDLER
            #SPRITE_HANDLER = SpriteHandler()

            sprite_id = background_jobj[JSON_KEY_BACKGROUND_SPRITE]
            solid = background_jobj[JSON_KEY_BACKGROUND_SOLID]
            sprite = SPRITE_HANDLER.get(sprite_id)

            parsed_background_sprites.append(sprite)          # TODO: get sprite via singleton handler
            parsed_background_solid.append(solid)

        lv = Level(parsed_id, parsed_width, parsed_height, parsed_background_sprites, parsed_background_solid)
        return lv

    def get_background(self, x, y):
        i = y * self.width + x
        return self.background[i]
        pass

    def get_object(self, x, y):
        i = y * self.width + y
        return None
        pass

def draw():
    # Do Nothing
    return

    #global SDL_RENDERER
    #SDL_SetRenderDrawColor(SDL_RENDERER, ctypes.c_ubyte(255), ctypes.c_ubyte(255),
    #                                ctypes.c_ubyte(255), ctypes.c_ubyte(255))
    #SDL_RenderClear(SDL_RENDERER)

    #x1 = ctypes.c_int(random.randrange(0, 600))
    #y1 = ctypes.c_int(random.randrange(0, 500))
    #x2 = ctypes.c_int(random.randrange(0, 600))
    #y2 = ctypes.c_int(random.randrange(0, 500))

    #r = ctypes.c_ubyte(random.randrange(0, 255))
    #g = ctypes.c_ubyte(random.randrange(0, 255))
    #b = ctypes.c_ubyte(random.randrange(0, 255))

    #SDL_SetRenderDrawColor(SDL_RENDERER, r, g, b, ctypes.c_ubyte(255))
    #SDL_RenderDrawLine(SDL_RENDERER, x1, y1, x2, y2)

    #render()

def render():
    # Update Tk
    root.update()

    # Clear SDL screen 
    global SDL_RENDERER
    global SDL_WINDOW
    SDL_SetRenderDrawColor(SDL_RENDERER,
                           ctypes.c_ubyte(255),
                           ctypes.c_ubyte(255),
                           ctypes.c_ubyte(255),
                           ctypes.c_ubyte(255))
    SDL_RenderClear(SDL_RENDERER)
    print("cleared screen")

    # Render all sprites
    global LEVEL
    global GRID_SIZE

    for y in range(LEVEL.height):
        for x in range(LEVEL.width):
            sprite = LEVEL.get_background(x, y)

            if sprite == None:
                print('Error when rendering SpriteSheet at ', end='', file=sys.stderr)
                print('(' +str(x) +',' +str(y) +')', file=sys.stderr)
                print('SpriteSheet is NULL')
                print()
                continue

            sprite.render(x * GRID_SIZE, y * GRID_SIZE)

    # Render to screen
    SDL_RenderPresent(SDL_RENDERER);

    #
    #global SDL_EVENT

    #event = SDL_Event()
    #if SDL_PollEvent(ctypes.byref(event)) != 0:
    #    if event.type == SDL_QUIT:
    #        SDL_DestroyRenderer(SDL_RENDERER)
    #        SDL_DestroyWindow(SDL_WINDOW)
    #        SDL_Quit()

def main():
    # Initialize SDL
    create_gui()

    # Initialize sprites
    global SPRITE_HANDLER
    SPRITE_HANDLER = SpriteHandler()

    # Read level
    global LEVEL
    json_file = LEVEL_DIR +'level.json'
    with open(json_file) as json_data:
        data = json.load(json_data)
        LEVEL = Level.from_json(data)

    root.after(0, render)
    root.mainloop()

    #while True:
    #    root.update()

    #while True:
    #    render()


def create_gui():
    # Tk window
    global root
    root = tk.Tk()
    root.title('Level Editor')

    # Set style
    if 'clam' in ttk.Style().theme_names():
        root.style = ttk.Style()
        root.style.theme_use('clam')

    # Menu bar
    menu = tk.Menu(root)
    root.config(menu = menu)

    file_menu = tk.Menu(menu, tearoff = 0)
    file_menu.add_command(label = 'Open')
    file_menu.add_command(label = 'Save')
    menu.add_cascade(label = 'File', menu = file_menu)

    # Left frame
    left_frame = ttk.Frame(root)
    left_frame.grid(column = 0, row = 0)

    # Scrollbar
    vscrollbar = ttk.Scrollbar(left_frame, orient = tk.VERTICAL)
    vscrollbar.pack(side = tk.RIGHT, fill = tk.Y, expand = tk.FALSE)

    scrolled_canvas = tk.Canvas(left_frame, bd = 0, highlightthickness = 0, yscrollcommand = vscrollbar.set)
    scrolled_canvas.pack(side = tk.LEFT, fill = tk.BOTH, expand = tk.TRUE)
    vscrollbar.config(command = scrolled_canvas.yview)

    #demolist = tk.Listbox(left_frame, yscrollcommand = scrollbar.set)
    #for i in range(100):
    #    demolist.insert(tk.END, 'foobar ' +str(i + 1))
    #demolist.pack(side = tk.TOP)
    #scrollbar.config(command = demolist.yview)

    scrolled_frame = tk.Frame(scrolled_canvas)
    button4 = tk.Button(scrolled_frame, text = 'Button 4')
    button4.pack(side = tk.TOP, padx = 10, pady = 10)
    button5 = tk.Button(scrolled_frame, text = 'Button 5')
    button5.pack(side = tk.TOP, padx = 10, pady = 10)

    # Level id label
    embed_lbl = ttk.Label(left_frame, text = 'Level View:')
    embed_lbl.pack(side = tk.TOP)

    # Create embed frame for pysdl window
    # This has to be a tk.Frame, not a ttk.Frame !
    embed_frame = tk.Frame(left_frame, width = 500, height = 500)
    embed_frame.pack(side = tk.TOP)

    # Right (button) frame
    button_frame = ttk.Frame(root, width = 150)
    button_frame.grid(column = 1, row = 0)

    refresh_button = ttk.Button(button_frame, text = 'Refresh', command = draw)
    refresh_button.pack(side = tk.TOP, padx = 10, pady = 10)

    button2 = ttk.Button(button_frame, text = 'Button 2')
    button2.pack(side = tk.TOP, padx = 10, pady = 10)

    button3 = ttk.Button(button_frame, text = 'Button 3')
    button3.pack(side = tk.TOP, padx = 10, pady = 10)

    root.update()

    # SDL2
    SDL_Init(SDL_INIT_VIDEO)
    SDL_WINDOW = SDL_CreateWindowFrom(embed_frame.winfo_id())

    global SDL_RENDERER
    SDL_RENDERER = SDL_CreateRenderer(SDL_WINDOW, -1, 0)

    #SDL_SetRenderDrawColor(SDL_RENDERER, ctypes.c_ubyte(0), ctypes.c_ubyte(0),
    #                                ctypes.c_ubyte(0), ctypes.c_ubyte(0))
    #SDL_RenderClear(SDL_RENDERER)

    global SDL_EVENT
    SDL_EVENT = SDL_Event()



if __name__ == '__main__':
    main()

