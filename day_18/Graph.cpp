#include "Graph.h"

#include "console_image.h"

std::string Graph::draw() const
{
	return generate_image(tileMap, ' ', true);
}


GraphBuilder::GraphBuilder(const Image & img) : m_img{ img }
{
	
}


void GraphBuilder::startExplore(const Tile & start)
{
	m_currentTile = start;

	if (m_graph.adjacencyList.find(m_currentTile) == m_graph.adjacencyList.end())
	{
		m_graph.adjacencyList[m_currentTile] = {};
	}

	if (m_visited.find(m_currentTile) == m_visited.end())
	{
		m_visited[m_currentTile] = {};
	}

	m_graph.tileMap[m_currentTile] = '.';

	explore();
}


bool GraphBuilder::advance(Direction d)
{
	Tile t = nextTile(m_currentTile, d);

	auto itVisited = m_visited.find(t);
	if(itVisited != m_visited.end())
	{
		auto itDirection = itVisited->second.find(d);
		if(itDirection != itVisited->second.end())
		{
			return false;
		}
		else
		{
			itVisited->second.insert(d);
		}

	}
	else
	{
		m_visited[t] = {d};
	};

	char result = m_img.at(t);

	if (result == '#')
	{
		return false;
	}

	if (m_graph.tileMap.find(t) == m_graph.tileMap.end())
	{
		m_graph.tileMap[t] = result;
	}

	m_graph.adjacencyList.at(m_currentTile).insert(t);

	if (m_graph.adjacencyList.find(t) == m_graph.adjacencyList.end())
	{
		m_graph.adjacencyList[t] = {};
	}

	m_graph.adjacencyList.at(t).insert(m_currentTile);

	if (result != '.')
	{
		m_graph.poiMap[result] = t;
	}

	m_currentTile = t;

	return true;
}


void GraphBuilder::explore(Direction previous)
{
	for (int di = 1; di <= 4; ++di)
	{
		const auto d = static_cast<Direction>(di);

		if (advance(d))
		{
			explore(d);
		}
	}

	if (previous != Still)
	{
		m_currentTile = nextTile(m_currentTile, inverse(previous));
	}
}


Graph GraphBuilder::buildGraph(const Image & img, const Tile & start)
{
	return buildGraph(img, std::vector<Tile>{ start });
}


Graph GraphBuilder::buildGraph(const Image & img, const std::vector<Tile> & start)
{
	GraphBuilder b{ img };

	for (const auto & s : start)
	{
		b.startExplore(s);
	}

	return b.m_graph;
}

