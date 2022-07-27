#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

constexpr float SCALE = 35.f;
float pixelsToMeters(const float px);
float metersToPixels(const float meters);

class DebugDraw final : public b2Draw
{
private:
	sf::RenderTarget& renderTarget;

public:
	explicit DebugDraw(sf::RenderTarget& renderTarget)
		: renderTarget(renderTarget)
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
		circle.setOrigin({ r, r });
		circle.setPosition(convert(center));
		circle.setOutlineThickness(2);
		circle.setOutlineColor(convert(color));
		renderTarget.draw(circle);
	}
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		const float r = metersToPixels(radius);
		sf::CircleShape circle(r);
		circle.setOrigin({ r, r });
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
		circle.setOrigin({ r / 2.f, r / 2.f });
		circle.setPosition(convert(p));
		circle.setFillColor(convert(color));
		renderTarget.draw(circle);
	}

	void DrawTransform(const b2Transform& xf) override
	{}

private:
	static sf::Vector2f convert(const b2Vec2& p)
	{
		return { metersToPixels(p.x), metersToPixels(p.y) };
	}
	static sf::Color convert(const b2Color& color)
	{
		auto c = [](const float v) -> sf::Uint8 {
			return static_cast<sf::Uint8>(std::floor(v >= 1.0f ? 255 : v * 256.0f));
		};
		return { c(color.r), c(color.g), c(color.b), c(color.a) };
	}
};
