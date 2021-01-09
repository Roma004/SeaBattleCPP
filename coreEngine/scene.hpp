#ifndef SCENE
#define SCENE

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <string>
#include <SFML/Window.hpp>

class scene {
private:
    bool shouldStop = false;

    void mainloop(sf::RenderWindow& window) {
        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) {
                    window.close();
                }

                if (e.type == sf::Event::MouseButtonPressed) {
                    mouseButtonPressedEvent(e);
                }

                if (e.type == sf::Event::MouseMoved) {
                    mouseMovedEvent(e);
                }

                if (e.type == sf::Event::MouseButtonReleased) {
                    mouseButtonRealisedEvent(e);
                }
            }

            window.clear();

            if (shouldStop) break;
        
            drawShapes(window);

            window.display();
        }
    }

public:
    virtual void drawShapes(sf::RenderWindow& window) {}
    virtual void mouseButtonPressedEvent(sf::Event e) {}
    virtual void mouseButtonRealisedEvent(sf::Event e) {}
    virtual void mouseMovedEvent(sf::Event e) {}

    scene() {}

    void start(sf::RenderWindow& window) {
        this->mainloop(window);
    }

    void stop() {
        this->shouldStop = true;
    }
};

#endif