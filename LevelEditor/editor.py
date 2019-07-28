#!/usr/bin/env python3

import os

import json

import random

import ctypes

import sdl2
from sdl2 import *

import tkinter as tk
from tkinter import *

LEVEL_DIR = '../levels/'
SPRITE_DIR = '../sprites/'

global SDL_RENDERER
global SDL_WINDOW
global SDL_EVENT
SDL_EVENT = SDL_Event()
SPRITE_HANDLER = None

def singleton(class_):
    instances = {}
    def getinstance(*args, **kwargs):
        if class_ not in instances:
            instances[class_] = class_(*args, **kwargs)
        return instances[class_]
    return getinstance

@singleton
class SpriteHandler:
    sprite_sheets = []

    def __init__(self):
        for filename in os.listdir(SPRITE_DIR):
            if not filename.endswith('.json'):
                continue
            print(filename)
        pass

class SpriteSheet:
    sprite_id = ""
    texture = None
    states = []

    def __init__(self, sprite_id, texture, states):
        self.sprite_id = sprite_id

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
        parsed_id = json[JSON_KEY_ID]
        parsed_texture = json[JSON_KEY_TEXTURE]

        # TODO: load texture

        pass

class Level:
    level_id = ""
    width = 0
    height = 0
    background = []
    solid = []

    def __init__(self, level_id, width, height, background, solid):
        self.level_id = level_id
        self.width = width
        self.height = height
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

            sprite_id = background_jobj[JSON_KEY_BACKGROUND_SPRITE]
            solid = background_jobj[JSON_KEY_BACKGROUND_SOLID]
            parsed_background_sprites.append(None)          # Todo get sprite via singleton handler
            parsed_background_solid.append(solid)

        lv = Level(parsed_id, parsed_width, parsed_height, parsed_background_sprites, parsed_background_solid)
        return lv


def draw():
    global SDL_RENDERER
    SDL_SetRenderDrawColor(SDL_RENDERER, ctypes.c_ubyte(255), ctypes.c_ubyte(255),
                                    ctypes.c_ubyte(255), ctypes.c_ubyte(255))
    SDL_RenderClear(SDL_RENDERER)

    x1 = ctypes.c_int(random.randrange(0, 600))
    y1 = ctypes.c_int(random.randrange(0, 500))
    x2 = ctypes.c_int(random.randrange(0, 600))
    y2 = ctypes.c_int(random.randrange(0, 500))

    r = ctypes.c_ubyte(random.randrange(0, 255))
    g = ctypes.c_ubyte(random.randrange(0, 255))
    b = ctypes.c_ubyte(random.randrange(0, 255))

    SDL_SetRenderDrawColor(SDL_RENDERER, r, g, b, ctypes.c_ubyte(255))
    #SDL_SetRenderDrawColor(SDL_RENDERER, ctypes.c_ubyte(255), ctypes.c_ubyte(255), ctypes.c_ubyte(255), ctypes.c_ubyte(255))
    SDL_RenderDrawLine(SDL_RENDERER, x1, y1, x2, y2)

    render()

def render():
    root.update()

    # SDL
    global SDL_RENDERER
    SDL_RenderPresent(SDL_RENDERER);

    global SDL_WINDOW
    #global SDL_EVENT

    event = SDL_Event()
    if SDL_PollEvent(ctypes.byref(event)) != 0:
        if event.type == SDL_QUIT:
            SDL_DestroyRenderer(SDL_RENDERER)
            SDL_DestroyWindow(SDL_WINDOW)
            SDL_Quit()

def main():
    # Initialize sprites
    SPRITE_HANDLER = SpriteHandler()

    # Read level
    json_file = LEVEL_DIR +'level.json'
    with open(json_file) as json_data:
        data = json.load(json_data)
        lv = Level.from_json(data)

    create_gui()

    root.after(0, render)
    root.mainloop()
    #while True:
    #    root.update()


def create_gui():
    # Tk
    global root
    root = tk.Tk()

    # Create embed frame for pysdl window
    embed_frame = tk.Frame(root, width = 500, height = 500)
    embed_frame.grid(columnspan = 600, rowspan = 500)  # Add grid
    embed_frame.pack(side = LEFT)                      # Pack window to the left

    button_frame = tk.Frame(root, width = 75, height = 500)
    button_frame.pack(side = LEFT)
    button1 = Button(button_frame, text = 'Draw',  command=draw)
    button1.pack(side = LEFT)

    root.update()

    # SDL2
    SDL_Init(SDL_INIT_VIDEO)
    SDL_WINDOW = SDL_CreateWindowFrom(embed_frame.winfo_id())

    global SDL_RENDERER
    SDL_RENDERER = SDL_CreateRenderer(SDL_WINDOW, -1, 0)

    SDL_SetRenderDrawColor(SDL_RENDERER, ctypes.c_ubyte(0), ctypes.c_ubyte(0),
                                    ctypes.c_ubyte(0), ctypes.c_ubyte(0))
    SDL_RenderClear(SDL_RENDERER)

    global SDL_EVENT
    SDL_EVENT = SDL_Event()



if __name__ == '__main__':
    main()

