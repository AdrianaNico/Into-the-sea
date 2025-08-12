
#include <iostream>
#include <SFML/Graphics.hpp>

int main(){

    unsigned int height = 600;
    unsigned int width = 800;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width, height}), "Into the Sea");
    //bubble
    sf::CircleShape bubble(30.f);
    bubble.setFillColor(sf::Color(173, 216, 230));
    bubble.setPosition(sf::Vector2f(100.f, 100.f));
    bubble.setOutlineColor(sf::Color::White);
    bubble.setOutlineThickness(2.f);

    //turtle
    sf::Texture turtleTexture;
    if(!turtleTexture.loadFromFile("turtle.png")){
        return -1;
    }

    sf::Sprite turtleSprite(turtleTexture);

    //scalam dimensiunea testoasei dupa dimensiunea window ului
    sf::Vector2u turtleSize= turtleTexture.getSize();
    float scaleTurtleX=200.f/ turtleSize.x;
    float scaleTurtleY=200.f/ turtleSize.y;

    turtleSprite.setScale({scaleTurtleX, scaleTurtleY});

    //background
    sf::Texture bgTexture;
    if(!bgTexture.loadFromFile("background.jpeg")){
        return -1;
    }

    sf::Sprite bgSprite(bgTexture);
    turtleSprite.setPosition({400.f, 300.f});

    //set the background to cover the entire window
    sf::Vector2u windowSize= window->getSize();
    sf::Vector2u textureSize= bgTexture.getSize();

    //calculam factorul de scalare pe x si y
    float scaleX= static_cast<float>(windowSize.x)/textureSize.x;
    float scaleY= static_cast<float>(windowSize.y)/textureSize.y;

    //aplicam scalarea
    bgSprite.setScale({scaleX,scaleY});

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

        //moving the bubble
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            bubble.move({-speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            bubble.move({speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
            bubble.move({ 0.f, -speed * dt});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
            bubble.move({ 0.f, speed * dt});

        //verificam marginile pentru bubble
        {
            auto bounds = bubble.getGlobalBounds();
            sf::Vector2f pos = bubble.getPosition();
            if (bounds.position.x < 0)
                pos.x = 0;
            if (bounds.position.y < 0)
                pos.y = 0;
            if (bounds.position.x + bounds.size.x > width)
                pos.x = width - bounds.size.x;
            if (bounds.position.y + bounds.size.y > height)
                pos.y = height - bounds.size.y;

            bubble.setPosition(pos);
        }

        //moving the turtle
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            turtleSprite.move({-speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            turtleSprite.move({speed * dt, 0.f});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
            turtleSprite.move({ 0.f, -speed * dt});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
            turtleSprite.move({ 0.f, speed * dt});

        //verificam marginile lui turtle 
        {
            auto bounds = turtleSprite.getGlobalBounds();
            sf::Vector2f pos = turtleSprite.getPosition();
            if (bounds.position.x < 0)
                pos.x = 0;
            if (bounds.position.y < 0)
                pos.y = 0;
            if (bounds.position.x + bounds.size.x > width)
                pos.x = width - bounds.size.x;
            if (bounds.position.y + bounds.size.y > height)
                pos.y = height - bounds.size.y;

            turtleSprite.setPosition(pos);
        }
            
        //render
        window->clear();


        //drawing
        window->draw(bgSprite);
        window->draw(bubble);
        window->draw(turtleSprite);

        window->display();
    }

    delete window;
    return 0;   
}