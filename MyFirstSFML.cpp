
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib> //pt rand()
#include <ctime> //pt time()
#include <vector>

int main(){

    std::srand(static_cast<unsigned>(std::time(nullptr)));//initializarea random

    unsigned int height = 600;
    unsigned int width = 800;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({width, height}), "Into the Sea");
    
    int lives=3;
    float heartScale=0.15f;
    float margin=1.f;
    float spacing=1.f;
    sf::Texture heartTexture;
    if(!heartTexture.loadFromFile("heart.png")){
        return -1;
    }

    std::vector<sf::Sprite> hearts;
    for( int i=0;i<3;i++){
        sf::Sprite heart(heartTexture);
        heart.setScale({heartScale, heartScale});
        heart.setPosition({margin + i * (heartTexture.getSize().x * heartScale + spacing), margin});
        hearts.push_back(heart);
    }
    //font
    // sf::Font font;
    // if(!font.loadFromFile("HennyPenny-Regular.ttf")){
    //     return -1;
    // }

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
\
    //window & bg size
    sf::Vector2u textureSize= bgTexture.getSize();
    sf::Vector2u windowSize= window->getSize();


    //start button
    sf::Texture StartTexture;
    if(!StartTexture.loadFromFile("startButton.png")){
        return -1;
    }
    sf::Sprite StartSprite(StartTexture);
    float StartScale=0.5f;
    StartSprite.setScale({StartScale, StartScale});
    sf::Vector2u startSize = StartTexture.getSize();
    StartSprite.setPosition({
        (windowSize.x - startSize.x * StartScale) / 2.f,
        (windowSize.y - startSize.y * StartScale) / 2.f
    });


    //set the background to cover the entire window

    //calculam factorul de scalare pe x si y
    float scaleX= static_cast<float>(windowSize.x)/textureSize.x;
    float scaleY= static_cast<float>(windowSize.y)/textureSize.y;

    //aplicam scalarea
    bgSprite.setScale({scaleX,scaleY});

    //directia random
    float dx=(std::rand() % 201-100) /50.f;
    float dy=(std::rand() % 201-100) /50.f;

    sf::Clock clock;
    sf::Clock changeDirectionClock;
    float speed =500.f;


    bool inGame= false;// suntem inca in meniul de start

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

                //scalarea bg ului
                sf::Vector2u newSize = window->getSize();
                sf::Vector2u textureSize = bgTexture.getSize();

                float scaleX = static_cast<float>(newSize.x) / textureSize.x;
                float scaleY = static_cast<float>(newSize.y) / textureSize.y;

                bgSprite.setScale({scaleX, scaleY});

                //update button
                StartSprite.setPosition({
                    (newSize.x - startSize.x * StartScale) / 2.f,
                    (newSize.y - startSize.y * StartScale) / 2.f,

                });
            }
            else if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
                if(mousePressed->button == sf::Mouse::Button::Left && !inGame){
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);//gasim "coord" in pixeli
                    sf::Vector2f mousePos = window->mapPixelToCoords(pixelPos);// convertim pixelii la coordonate
                    if(StartSprite.getGlobalBounds().contains(mousePos)){
                        inGame=true;
                        lives=3;
                    }
                }
            }

        }

        float dt=clock.restart().asSeconds();//timpul scurs de la ultimul frame

        //moving the bubble random
        if(changeDirectionClock.getElapsedTime().asSeconds()>2.f){
            dx = (std::rand() % 201 - 100) / 50.f;
            dy = (std::rand() % 201 - 100) / 50.f;
            changeDirectionClock.restart();
        }

        bubble.move({dx* speed * dt, dy* speed* dt});

        //verificam marginile pentru bubble
        {
            auto bounds = bubble.getGlobalBounds();
            sf::Vector2f pos = bubble.getPosition();
            if (bounds.position.x < 0)
                {pos.x = 0;
                dx=-dx;}
            if (bounds.position.y < 0)
                {pos.y = 0;
                dy=-dy;}
            if (bounds.position.x + bounds.size.x > window->getSize().x)
                {pos.x = window->getSize().x - bounds.size.x;
                dx=-dx;}
            if (bounds.position.y + bounds.size.y > window->getSize().y)
                {pos.y = window->getSize().y - bounds.size.y;
                dy=-dy;}

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
            if (bounds.position.x + bounds.size.x > window->getSize().x)
                pos.x = window->getSize().x - bounds.size.x;
            if (bounds.position.y + bounds.size.y > window->getSize().y)
                pos.y = window->getSize().y - bounds.size.y;

            turtleSprite.setPosition(pos);
        }
        

        if(inGame){


        }
        //render
        window->clear();


        //drawing
        window->draw(bgSprite);
        window->draw(turtleSprite);
        if(!inGame){
            window->draw(StartSprite);
        }
        else{
            window->draw(bubble);
        }
        for (int i = 0; i < lives; ++i) {
            window->draw(hearts[i]);
}

        window->display();
    }

    delete window;
    return 0;   
}