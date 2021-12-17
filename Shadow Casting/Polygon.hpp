#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

struct Edge 
{
	sf::Vector2f Start;
	sf::Vector2f End;
};

class Polygon : public sf::Drawable {
public:
	Edge*		   Edges	  = nullptr;
	const uint32_t EdgesCount = 0;

	Polygon(uint32_t count) :
		EdgesCount(count) 
	{
		Edges	   = new Edge[count];
		m_vertices = new sf::Vertex[count + 1]; // We need to add one for last vertice
	}
	
	~Polygon() {
		delete[] m_vertices;
		delete[] Edges;
	}

private:
	sf::Vertex* m_vertices = nullptr;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Because one Edge has two variables which can use to define two separate Vertex 
		// we can iterate every two
		for (uint32_t i = 0; i <= EdgesCount - 1; i+=2) {
			m_vertices[i]	  = Edges[i].Start;
			m_vertices[i + 1] = Edges[i].End;
		}

		// Final vertex to complete shape. Here you can see why we created m_vertices with size "count + 1"
		// now we can use that to close our polygon
		m_vertices[EdgesCount] = Edges[EdgesCount - 1].End;

		target.draw(m_vertices, EdgesCount + 1, sf::LineStrip, states);
	}
};