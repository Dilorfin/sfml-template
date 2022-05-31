#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <box2d/box2d.h>

constexpr float SCALE = 35.f;
float pixelsToMeters(const float px)
{
	return px / SCALE;
}

float metersToPixels(const float meters)
{
	return meters * SCALE;
}

class DebugDraw final : public b2Draw
{
private:
	sf::RenderTarget& renderTarget;
public:
	explicit DebugDraw(sf::RenderTarget& renderTarget)
		:renderTarget(renderTarget)
	{}

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::VertexArray lines(sf::LineStrip, vertexCount + 1);
		for (int i = 0; i < vertexCount; ++i)
		{
			lines[i].position = convert(vertices[i]);
			lines[i].color = convert(color);
		}
		lines[vertexCount].position = convert(vertices[0]);
		lines[vertexCount].color = convert(color);
		renderTarget.draw(lines);
	}
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::VertexArray lines(sf::TriangleStrip, vertexCount + 1);
		for (int i = 0; i < vertexCount; ++i)
		{
			lines[i].position = convert(vertices[i]);
			lines[i].color = convert(color);
		}
		lines[vertexCount].position = convert(vertices[0]);
		lines[vertexCount].color = convert(color);
		renderTarget.draw(lines);
	}
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
	{
		const float r = metersToPixels(radius);
		sf::CircleShape circle(r);
		circle.setOrigin({r, r});
		circle.setPosition(convert(center));
		circle.setOutlineThickness(2);
		circle.setOutlineColor(convert(color));
		renderTarget.draw(circle);
	}
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		const float r = metersToPixels(radius);
		sf::CircleShape circle(r);
		circle.setOrigin({r, r});
		circle.setPosition(convert(center));
		circle.setFillColor(convert(color));
		renderTarget.draw(circle);
	}
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
	{
		sf::VertexArray lines(sf::Lines, 2);
		lines[0].position = convert(p1);
		lines[0].color = convert(color);
		lines[1].position = convert(p2);
		lines[1].color = convert(color);
		renderTarget.draw(lines);
	}

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override
	{
		const float r = metersToPixels(size);
		sf::CircleShape circle(r);
		circle.setOrigin({r / 2.f, r / 2.f});
		circle.setPosition(convert(p));
		circle.setFillColor(convert(color));
		renderTarget.draw(circle);
	}

	void DrawTransform(const b2Transform& xf) override
	{}

private:
	static sf::Vector2f convert(const b2Vec2& p)
	{
		return {
			metersToPixels(p.x),
			metersToPixels(p.y)
		};
	}
	static sf::Color convert(const b2Color& color)
	{
		auto c = [](const float v)->sf::Uint8 {
			return static_cast<sf::Uint8>(std::floor(v >= 1.0f ? 255 : v * 256.0f));
		};
		return {
			c(color.r),
			c(color.g),
			c(color.b),
			c(color.a)
		};
	}
};

void CreateGround(b2World* world, float X, float Y, float angle);
void CreateBox(b2World* world, int MouseX, int MouseY);
void CreateCircle(b2World* world, int MouseX, int MouseY);

// Do we want to showcase direct JNI/NDK interaction?
// Undefine this to get real cross-platform code.
// Uncomment this to try JNI access; this seems to be broken in latest NDKs
//#define USE_JNI

#if defined(USE_JNI)
// These headers are only needed for direct NDK/JDK interaction
#include <jni.h>
#include <android/native_activity.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>

// NDK/JNI sub example - call Java code from native code
int vibrate(sf::Time duration)
{
    // First we'll need the native activity handle
    ANativeActivity *activity = sf::getNativeActivity();

    // Retrieve the JVM and JNI environment
    JavaVM* vm = activity->vm;
    JNIEnv* env = activity->env;

    // First, attach this thread to the main thread
    JavaVMAttachArgs attachargs;
    attachargs.version = JNI_VERSION_1_6;
    attachargs.name = "NativeThread";
    attachargs.group = nullptr;
    jint res = vm->AttachCurrentThread(&env, &attachargs);

    if (res == JNI_ERR)
        return EXIT_FAILURE;

    // Retrieve class information
    jclass natact = env->FindClass("android/app/NativeActivity");
    jclass context = env->FindClass("android/content/Context");

    // Get the value of a constant
    jfieldID fid = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
    jobject svcstr = env->GetStaticObjectField(context, fid);

    // Get the method 'getSystemService' and call it
    jmethodID getss = env->GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);

    // Get the object's class and retrieve the member name
    jclass vib_cls = env->GetObjectClass(vib_obj);
    jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V");

    // Determine the timeframe
    jlong length = duration.asMilliseconds();

    // Bzzz!
    env->CallVoidMethod(vib_obj, vibrate, length);

    // Free references
    env->DeleteLocalRef(vib_obj);
    env->DeleteLocalRef(vib_cls);
    env->DeleteLocalRef(svcstr);
    env->DeleteLocalRef(context);
    env->DeleteLocalRef(natact);

    // Detach thread again
    vm->DetachCurrentThread();
}
#endif

// This is the actual Android example. You don't have to write any platform
// specific code, unless you want to use things not directly exposed.
// ('vibrate()' in this example; undefine 'USE_JNI' above to disable it)
int main(int argc, char *argv[])
{
    sf::VideoMode screen(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    sf::Texture texture;
    if(!texture.loadFromFile("image.png"))
        return EXIT_FAILURE;

    sf::Sprite image(texture);

    image.setPosition({screen.size.x / 2.f, screen.size.y / 2.f});
    image.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});

    sf::Font font;
    if (!font.loadFromFile("tuffy.ttf"))
        return EXIT_FAILURE;

    sf::Text text("Tap anywhere to move the logo.", font, 64);
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