
#include <iostream>
#include <SFML/Graphics.hpp>

int main(){

    unsigned int height = 600;
    unsigned int width = 800;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width, height}), "Move the Circles");
    sf::CircleShape circle1(50.f);
    circle1.setFillColor(sf::Color(255, 182, 193));
    circle1.setPosition(sf::Vector2f(100.f, 100.f)); 

    sf::Clock clock;
    float speed =500.f;

    while(window->isOpen()){

        while(const std::optional event =  window->pollEvent()){

            if(event->is<sf::Event::Closed>()){
                window->close();
            }
            else if( const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape){
                    window->close();
                }
            }
            else if(event->is<sf::Event::Resized>()){
                //update view
                sf::View view(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window->getSize())));
                window->setView(view);
            }

        }

        float dt=clock.restart().asSeconds();//timpul scurs de la ultimul frame


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            circle1.move({-speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            circle1.move({speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            circle1.move({ 0.f, -speed * dt});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
            circle1.move({ 0.f, speed * dt});

        //render
        window->clear();

        //drawing
        window->draw(circle1);

        window->display();
    }

    delete window;
    return 0;   
}