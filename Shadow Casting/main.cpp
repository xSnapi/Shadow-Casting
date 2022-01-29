/*
	Simplest "Shadow Casting" implementation by Snapi
*/

#include <iostream>
#include <vector>
#include <algorithm>

#include "Polygon.hpp"
#include "Point.hpp"

#include <SFML/Graphics.hpp>

// Set to 1 to see how caster works with shaders
#define SHADER_ON 0

// uint32_t is basically "unsigned int" or just "unsigned" (they are the same)
// i use it a lot so don't get scared
constexpr uint32_t WINDOW_WIDTH  = 800;
constexpr uint32_t WINDOW_HEIGHT = 800;

// Function initializing our shapes. Note that "&" here is very important
void InitPolygons(std::vector<Polygon>& Polygons);

// This function initializes our Edge vector with all edges stored in Polygon vector
void InitEdges(std::vector<Edge>& Edges, std::vector<Polygon>& Polygons);

int main() {
	// In this vector we store our shapes
	std::vector<Polygon> Polygons;
	InitPolygons(Polygons);

	std::vector<Edge> Edges;
	InitEdges(Edges, Polygons);

	std::vector<Point> Points;
	Points.reserve(500);

	// I know that SFML has build in class for it. I just like doing it on my own
	std::vector<sf::Vertex> Vertices;
	Vertices.reserve(500);

	sf::Shader shadowShader;
	shadowShader.loadFromFile("shadow.vert", "shadow.frag");
	shadowShader.setUniform("u_resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

	sf::RenderTexture castTexture;
	castTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	castTexture.setSmooth(true);

	// Context Settings for antialiasing
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;


	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Casting", sf::Style::Close, settings);
	window.setFramerateLimit(144);

	sf::Event event;

	// Here our application starts
	while (window.isOpen()) {

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		
		sf::Vector2f mousePos(sf::Mouse::getPosition(window));

		shadowShader.setUniform("u_mouse", mousePos);

		Points.clear();
		// Here maigc happens it's our line casting algorithm
		// It's author and better explanation can be found here
		// https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_ShadowCasting2D.cpp

		for (auto& e0 : Edges) {
			// Because we have two points in every edge we need to iterate through it like this
			for (uint32_t i = 0; i < 2; i++) {
				// Calculating vector between mouse and point of our edge
				sf::Vector2f rd((!i ? e0.Start : e0.End) - mousePos);

				float baseAng = atan2f(rd.y, rd.x);

				float ang = 0.0f;

				// For casting aditional rays
				for (uint32_t j = 0; j < 3; j++) {
					if (j == 0)	ang = baseAng - 0.0001f;
					if (j == 1)	ang = baseAng;
					if (j == 2)	ang = baseAng + 0.0001f;

					rd.x = 100.0f * cosf(ang);
					rd.y = 100.0f * sinf(ang);

					sf::Vector2f minP;
					float		 minT1	= 9999.0f;
					float		 minAng = 0.0f;
					bool		 valid	= false;

					for (auto& e1 : Edges) {
						// Algorithm detecting vector crossing
						// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

						sf::Vector2f sd = e1.End - e1.Start;

						float t2 = (rd.x * (e1.Start.y - mousePos.y) + (rd.y * (mousePos.x - e1.Start.x))) / (sd.x * rd.y - sd.y * rd.x);

						float t1 = (e1.Start.x + sd.x * t2 - mousePos.x) / rd.x;
						
						if (t1 > 0 && t2 >= 0 && t2 <= 1.0f) {
							if (t1 < minT1) {
								// If vectors cross we add thier intersection point to our vector 

								minT1  = t1;
								minP   = mousePos + rd * t1;
								minAng = atan2f(minP.y - mousePos.y, minP.x - mousePos.x);

								valid  = true;
							}
						}
					}

					if (valid)
						Points.emplace_back(sf::Vector2f(minP.x, minP.y),  minAng);
				}
			}
		}

		// We need to sort our points based on thier angle to draw them correctly
		std::sort(Points.begin(), Points.end(), [&](const Point& a, const Point& b) { return a.Angle < b.Angle; });

		window.clear(sf::Color(25, 94, 46));

		// Not sure about this part here maybe it can be done better
		castTexture.clear();
		Vertices.clear();

		// Center of triangle fan
		Vertices.emplace_back(mousePos);

		for (uint32_t i = 0; i < Points.size(); i++)
			Vertices.emplace_back(Points[i].Pos);

		if(!Points.empty())
			Vertices.emplace_back(Points[0].Pos);

		castTexture.draw(Vertices.data(), Vertices.size(), sf::TriangleFan);
		castTexture.display();	

		#if SHADER_ON
			window.draw(sf::Sprite(castTexture.getTexture()), &shadowShader);
		#else
			window.draw(sf::Sprite(castTexture.getTexture()));
		#endif

		// Drawing shapes
		for (auto& p : Polygons)
			window.draw(p);

		window.display();
	}

	return EXIT_SUCCESS;
}

void InitPolygons(std::vector<Polygon>& Polygons) {
	// Because of heap allocation in our Polygon class we need to 
	// reserve how many objects we want in our vector to avoid copy constructors

	Polygons.reserve(8); // <- it means 8 shapes (it's always better to reserve more then less)

	// Square 0
	{
		// Creates Polygon with 4 vertices
		auto& p = Polygons.emplace_back(4);

		// Note: start edge goes first
		p.Edges[0] = { sf::Vector2f(200.0f, 200.0f), sf::Vector2f(300.0f, 200.0f) };
		p.Edges[1] = { sf::Vector2f(300.0f, 200.0f), sf::Vector2f(300.0f, 300.0f) };
		p.Edges[2] = { sf::Vector2f(300.0f, 300.0f), sf::Vector2f(200.0f, 300.0f) };
		p.Edges[3] = { sf::Vector2f(200.0f, 300.0f), sf::Vector2f(200.0f, 200.0f) };
	}

	// Square 1
	{
		auto& p = Polygons.emplace_back(4);

		p.Edges[0] = { sf::Vector2f(500.0f, 600.0f), sf::Vector2f(600.0f, 600.0f) };
		p.Edges[1] = { sf::Vector2f(600.0f, 600.0f), sf::Vector2f(600.0f, 700.0f) };
		p.Edges[2] = { sf::Vector2f(600.0f, 700.0f), sf::Vector2f(500.0f, 700.0f) };
		p.Edges[3] = { sf::Vector2f(500.0f, 700.0f), sf::Vector2f(500.0f, 600.0f) };
	}

	// Square 2
	{
		auto& p = Polygons.emplace_back(4);

		p.Edges[0] = { sf::Vector2f(100.0f, 500.0f), sf::Vector2f(250.0f, 500.0f) };
		p.Edges[1] = { sf::Vector2f(250.0f, 500.0f), sf::Vector2f(250.0f, 650.0f) };
		p.Edges[2] = { sf::Vector2f(250.0f, 650.0f), sf::Vector2f(100.0f, 650.0f) };
		p.Edges[3] = { sf::Vector2f(100.0f, 650.0f), sf::Vector2f(100.0f, 500.0f) };
	}

	// Square 3 (Walls)
	{
		auto& p = Polygons.emplace_back(4);

		p.Edges[0] = { sf::Vector2f(0.0f,		  0.0f         ), sf::Vector2f(WINDOW_WIDTH, 0.0f		  )	};
		p.Edges[1] = { sf::Vector2f(WINDOW_WIDTH, 0.0f         ), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) };
		p.Edges[2] = { sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(0.0f,		 WINDOW_HEIGHT) };
		p.Edges[3] = { sf::Vector2f(0.0f,		  WINDOW_HEIGHT), sf::Vector2f(0.0f,		 0.0f		  )	};
	}

	// Triangle
	{
		auto& p = Polygons.emplace_back(3);

		p.Edges[0] = { sf::Vector2f(640.0f, 200.0f), sf::Vector2f(700.0f, 320.0f) };
		p.Edges[1] = { sf::Vector2f(700.0f, 320.0f), sf::Vector2f(500.0f, 250.0f) };
		p.Edges[2] = { sf::Vector2f(500.0f, 250.0f), sf::Vector2f(640.0f, 200.0f) };
	}
}

void InitEdges(std::vector<Edge>& Edges, std::vector<Polygon>& Polygons) {
	Edges.reserve(50);

	for (auto& p : Polygons)
		for (uint32_t i = 0; i < p.EdgesCount; i++)
			Edges.push_back(p.Edges[i]);
}