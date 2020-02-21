//
// Created by felix on 10.01.20.
//

#include "../include/Window.hpp"
using std::cout;
using std::endl;
using std::stringstream;

void Window::reset() {
    this->windowName = this->cpu->memory.rom.getGameName();

    clearScreen();
}

Window::Window(class CPU* cpu) {
    this->cpu = cpu;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "[Error] SDL coult not be initialised! SDL Error: " << SDL_GetError() << endl;
    }

    this->window = SDL_CreateWindow(
            windowName.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_ALLOW_HIGHDPI);

    this->renderer = SDL_CreateRenderer(
            this->window,
            -1,
            SDL_RENDERER_ACCELERATED);

    if (window == NULL || renderer == NULL) {
        cout << "[Error] window couldn't be created! Error: " << SDL_GetError() << endl;
    }

    SDL_RenderClear(renderer);
    clearScreen();
}

Window::~Window() {
    SDL_DestroyRenderer(this->getRenderer());
    SDL_DestroyWindow(this->getWindow());
    SDL_Quit();
}

void Window::set(int y, int x, int c) {
    if ((y >= 0 && y < Y_RES) && (x >= 0 && x < X_RES)) {
        this->display[y][x] = c;
    }
}

void Window::clearScreen() {
    SDL_RenderSetScale(
            renderer,
            (float)scaleWidth,
            (float)scaleHeight);

    for (int x = 0; x < X_RES; x++) {
        for (int y = 0; y < Y_RES; y++) {
            this->display[y][x] = 0;
            SDL_SetRenderDrawColor(renderer, white);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);

    return;
}

void Window::renderLine(Byte currentline) {
    SDL_RenderSetScale(
            renderer,
            (float)scaleWidth,
            (float)scaleHeight);

    for (int i = 0; i < X_RES; i++) {
        switch (display[currentline][i]) {
            case WHITE:
                SDL_SetRenderDrawColor(renderer, white);
                break;
            case LIGHT_GREY:
                SDL_SetRenderDrawColor(renderer, light_grey);
                break;
            case DARK_GREY:
                SDL_SetRenderDrawColor(renderer, dark_grey);
                break;
            case BLACK:
                SDL_SetRenderDrawColor(renderer, black);
                break;
            default: break;
        }

        SDL_RenderDrawPoint(renderer, i, currentline);
    }

    return;
}

void Window::render() {
    SDL_RenderPresent(renderer);

    return;
}

/**
 * @brief  saves the current SDL2 window to a bmp file.
 * @note   uses time() to get a unique name for each screenshot.
 * @retval None
 */
void Window::screenshot() {
    int w, h;
    time_t c;
    time(&c);
    stringstream ss;
    string name;
    ss << c;
    name = ss.str() + ".bmp";
    cout << "Your screenshot was saved as " << name << "." << endl;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_Surface *shot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, shot->pixels, shot->pitch);
    SDL_SaveBMP(shot, name.c_str());
    SDL_FreeSurface(shot);
}
