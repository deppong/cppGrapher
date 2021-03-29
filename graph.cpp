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
    gp -> minRange      = -10.0f;
    gp -> maxRange      =  10.0f;
    gp -> interations   = 500.0f;
    gp -> step          =    .005f;
    gp -> domain        = 20.0f;
    gp -> range         = 20.0f;
};

void drawAxes(graphParams &gp, VertexArray &axes) {
    axes[0].position = Vector2f(gp.domain, 0.0f);
    axes[1].position = Vector2f(-gp.domain, 0.0f);
    axes[2].position = Vector2f(0.0f, gp.range);
    axes[3].position = Vector2f(0.0f, -gp.range);
    
    for (int i = 0; i < 4; i++)
        axes[i].color = Color::Black;
}

void drawWindow(Font &font) {
    graphParams gp;
    RenderWindow window(VideoMode(width, height), "Polar Graph");
    float decFactor = 10.0f;
    float stretchFac = 10.0f;

    View view;
    view.setCenter(0.0f, 0.0f);
    VertexArray axes(Lines, 4);

    initGraph(&gp, view);

    while(window.isOpen()) { //CORE LOOP
        Event event;
        VertexArray points(LineStrip, gp.interations);

        // ======= Calculations ======
        points.clear();
        view.setSize(gp.domain, gp.range);

        for (float x = gp.minRange; x < gp.maxRange; x += gp.step) {
            points.append(Vertex(Vector2f(x, - tan(1/x)), Color::Blue));
        }    

        drawAxes(gp, axes);

        while(window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::K:
                        if(event.key.shift) {
                            if(gp.range == stretchFac)
                                stretchFac *=.5f;
                            gp.range += stretchFac;
                        } else {
                            if (gp.domain == decFactor && gp.range == decFactor ) {
                                decFactor *= 0.5f; 
                            }
                            gp.domain -= decFactor;
                            gp.range -= decFactor;
                            gp.minRange = -gp.domain / 2;
                            gp.maxRange = gp.domain / 2;
                            gp.interations += 100.0f;
                        }
                        break;
                    case Keyboard::J:
                        if (event.key.shift) {
                            if (stretchFac < 10.0f)
                                stretchFac *= 2.0f;
                            gp.range -= 10.0f;
                        } else {
                            if (decFactor < 10.0f) 
                                decFactor *= 2.0f; //undo the shrinkage of the decrementing
                            gp.domain += decFactor;
                            gp.range += decFactor;
                            gp.minRange = -gp.domain / 2;
                            gp.maxRange = gp.domain / 2;
                        }
                        break;
                    case Keyboard::H:
                        if (event.key.shift) 
                            gp.domain -= 10.0f;
                        break;
                    case Keyboard::L:
                        if (event.key.shift)
                            gp.domain += 10.0f;
                        break;
                    case Keyboard::P:
                        initGraph(&gp, view);
                        break;
                    case Keyboard::Up:
                        view.move(0.0f, -10.0f);
                        break;
                    case Keyboard::Down:
                        view.move(0.0f, 10.0f);
                        break;
                    case Keyboard::Left:
                        view.move(-10.0f, 0.0f);
                        gp.minRange += 10.0f;
                        gp.maxRange -= 10.0f;
                        break;
                    case Keyboard::Right:
                        view.move(10.0f, 0.0f);
                        gp.minRange -= 10.0f;
                        gp.maxRange += 10.0f;
                        break;
                    default:
                        break;
                }

            }
        }

        // ==== Drawing to Window ====
        window.setView(view);
        window.clear(Color::White);
        window.draw(axes);
        window.draw(points);
        window.display();
    }

}