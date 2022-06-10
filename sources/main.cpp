#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <box2d/box2d.h>
#include "debug.hpp"

void CreateGround(b2World* world, float X, float Y, float angle);
void CreateBox(b2World* world, int MouseX, int MouseY);
void CreateCircle(b2World* world, int MouseX, int MouseY);

int main(int argc, char *argv[])
{
    sf::VideoMode screen(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    sf::Texture texture;
    if(!texture.loadFromFile("assets/image.png"))
        return EXIT_FAILURE;

	std::string textData = "undefined";
#ifdef ANDROID_OS
	textData = "android";
#else
#ifdef WINDOWS_OS
	textData = "windows";
#else
	textData = "linux";
#endif
#endif

    sf::Sprite image(texture);

    image.setPosition({screen.size.x / 2.f, screen.size.y / 2.f});
    image.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});

    sf::Font font;
    if (!font.loadFromFile("assets/tuffy.ttf"))
        return EXIT_FAILURE;

    sf::Text text(textData, font, 64);
    text.setFillColor(sf::Color::Black);
    text.setPosition({10, 10});

	b2Vec2 gravity(0.f, 9.8f);
	b2World* world = new b2World(gravity);
	DebugDraw* debug = new DebugDraw(window);
	world->SetDebugDraw(debug);
	debug->AppendFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);

	CreateGround(world, 400, 300, 0);

	CreateBox(world, 15, 15);
	CreateCircle(world, 100, 100);

    sf::View view = window.getDefaultView();

    sf::Color background = sf::Color::White;

    // We shouldn't try drawing to the screen while in background
    // so we'll have to track that. You can do minor background
    // work, but keep battery life in mind.
    bool active = true;

    while (window.isOpen())
    {
        sf::Event event;

        while (active ? window.pollEvent(event) : window.waitEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(sf::Vector2f(event.size.width, event.size.height));
                    view.setCenter(sf::Vector2f(event.size.width / 2.f, event.size.height / 2.f));
                    window.setView(view);
                    break;
                case sf::Event::LostFocus:
                    background = sf::Color::Black;
                    break;
                case sf::Event::GainedFocus:
                    background = sf::Color::White;
                    break;

                // On Android MouseLeft/MouseEntered are (for now) triggered,
                // whenever the app loses or gains focus.
                case sf::Event::MouseLeft:
                    active = false;
                    break;
                case sf::Event::MouseEntered:
                    active = true;
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0)
                    {
                        image.setPosition(sf::Vector2f(static_cast<float>(event.touch.x), static_cast<float>(event.touch.y)));
                    }
                    break;
            }
        }

		world->Step(1 / 60.f, 8, 3);

        if (active)
        {
            window.clear(background);
            window.draw(image);
            window.draw(text);
			world->DebugDraw();
            window.display();
        }
        else {
            sf::sleep(sf::milliseconds(100));
        }
    }
}

void CreateCircle(b2World* world, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	
	b2Body* Body = world->CreateBody(&BodyDef);

	b2CircleShape circle;
	circle.m_radius = 15.0f / SCALE;

	b2FixtureDef FixtureDef;
	FixtureDef.density = 2.0f;
	FixtureDef.friction = 2.0f;
	FixtureDef.restitution = 0.5f;

	FixtureDef.shape = &circle;

	Body->CreateFixture(&FixtureDef);
}

void CreateBox(b2World* world, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 5.f;
	FixtureDef.friction = 5.0f;
	FixtureDef.restitution = 0.0f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World* world, float X, float Y, float angle)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	BodyDef.angle = angle;
	b2Body* Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((500.0f / 2) / SCALE, (16.0f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}