#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <SFML/Window.hpp>

using namespace sf;

static int width = 800;
static int height = 800;

struct graphParams {
    float minRange;
    float maxRange;
    float interations;
    float step;
    float domain;
    float range;
};

void initGraph(graphParams *gp, View &view) {
    view.setCenter(0.0f, 0.0f);
    view.setSize(width*.03f, height*.03f);
    gp -> minRange      = -100.0f;
    gp -> maxRange      =  100.0f;
    gp -> interations   =  500.0f;
    gp -> step          =    .005f;
    gp -> domain        = view.getSize().x;
    gp -> range         = view.getSize().y;
};

/*
void drawAxes(graphParams &gp, VertexArray &axes, View &view) {
    axes[0].position = Vector2f(gp.domain, 0.0f);
    axes[1].position = Vector2f(-gp.domain, 0.0f);
    axes[2].position = Vector2f(0.0f, gp.range);
    axes[3].position = Vector2f(0.0f, -gp.range);
    
    for (int i = 0; i < 4; i++)
        axes[i].color = Color::Black;
}
*/

void initWindow(Font &font, Text &text, RenderWindow &window, View &view, graphParams &gp) {
    // ===== Text ===== 
    text.setFont(font);
    text.setString("Function");
    text.setPosition(1000, 1000);
    text.setOrigin(1000, 1000);
    text.setCharacterSize(80);
    text.setFillColor(Color::White);

    view.setCenter(0.0f, 0.0f);
    view.setViewport(FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

    initGraph(&gp, view);
}

void drawWindow(Font &font) {
    // ==== var init ====
    Text text;
    graphParams gp;
    RenderWindow window(VideoMode(width, height), "C++ Graph");
    View view;
    float zoom = 1;
    Vector2f oldPos;
    bool moving = false;

    initWindow(font, text, window, view, gp);
    View textView(Vector2f(1000, 1000), Vector2f(150, 150));
    textView.setCenter(1000, 1000);
    textView.setViewport(FloatRect(0.0f, 0.0f, 0.15f, 0.15f));

    while(window.isOpen()) { //CORE LOOP
        Event event;
        VertexArray points(LineStrip, gp.interations);
        text.setCharacterSize(24);

        // ======= Calculations ======
        points.clear();
        gp.domain = 2 * view.getCenter().x;
        gp.range = 2 * view.getSize().y;
        gp.minRange = view.getCenter().x + (view.getSize().x - (2 * view.getSize().x));
        gp.maxRange = view.getCenter().x + view.getSize().x;

        for (float x = gp.minRange; x < gp.maxRange; x += gp.step) {
            points.append(Vertex(Vector2f(x, - sin(pow(x, 2))), Color::Blue));
        }    

        while(window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::KeyPressed:
                   switch (event.key.code) {
                        case Keyboard::R:
                            initGraph(&gp, view);
                            zoom = 1;
                            break;
                        case Keyboard::Q:
                            window.close();
                    }   
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == 0) {
                        moving = true; 
                        oldPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
                    }
                    break;
                case Event::MouseButtonReleased:
                    if (event.mouseButton.button == 0) {
                        moving = false;
                    }
                    break;
                case Event::MouseMoved:
                    {
                        if (!moving)
                            break;
                        Vector2f newPos = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));
                        Vector2f deltaPos = oldPos - newPos;
                        view.setCenter(view.getCenter() + deltaPos);
                        window.setView(view);

                        oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    }
                case sf::Event::MouseWheelScrolled:
                    if (moving)
                        break;

                    if (event.mouseWheelScroll.delta <= -1)
                        zoom = std::min(10.f, zoom + .1f);
                    else if (event.mouseWheelScroll.delta >= 1)
                        zoom = std::max(.05f, zoom - .1f);
                        

                    view.setSize(height * .03f, width * .03f);
                    view.zoom(zoom); 
                    window.setView(view);
                    break;
            }
        }

        // ==== Drawing to Window ====
        window.clear(Color::Black);

        window.setView(textView);
        window.draw(text);

        window.setView(view);
        //window.draw(axes);
        window.draw(points);

        window.display();
    }

}