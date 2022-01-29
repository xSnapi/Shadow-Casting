#pragma once

#include <SFML/System/Vector2.hpp>

struct Point
{
	sf::Vector2f Pos;
	float		 Angle = 0.0f;

	Point() {

	}

	Point(sf::Vector2f pos, float ang) 
		: Pos(pos)
		, Angle(ang)
	{

	}

	~Point() {

	}
};