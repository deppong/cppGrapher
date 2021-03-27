#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "graph.hpp"
#include <iostream>

using namespace sf;

Font font;

int fontLoader(Font &font) {
    if (!font.loadFromFile("fonts/arial.ttf")){
        return 1;
    }
    return 0;
}

int main() {
    if (fontLoader(font) == 0) {
        drawWindow(font);
    } else {
        printf("ERROR FONT NOT LOADED TRY LAUNCHING THE PROGRAM AGAIN OR MAKE SURE FILE \"fonts/arial.ttf\" EXISTS.\n PRESS ENTER TO EXIT.");
        while (std::cin.get() != '\n');
        return 1;
    }
    return 0;
}