/*
	Shadow Casting implementation by Mateusz Kozdrowicki
	currently working only with static shapes
	but there is quick fix for that (maybe some day)
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Polygon.hpp"

// uint32_t is basically "unsigned int" or just "unsigned" (they mean the same)
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

	sf::ContextSettings settings;

	//TODO: add antyaliasing
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Casting", sf::Style::Close, settings);
	window.setFramerateLimit(144);

	sf::Event event;

	// Main loop
	while (window.isOpen()) {

		// Simple pool event like always
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}



		window.clear();

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

	Polygons.reserve(8); // <- 8 means 8 shapes (it's always better to reserve more then less)

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
	for (auto& p : Polygons)
		for (uint32_t i = 0; i < p.EdgesCount; i++)
			Edges.push_back(p.Edges[i]);
}
