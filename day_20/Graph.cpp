#include "Graph.h"

#include <iostream>

#include "console_image.h"

std::string Graph::draw() const
{
	return generate_image(tileMap, ' ', true);
}


GraphBuilder::GraphBuilder(const Image & img) : m_img{ img }, m_portals{ findPortals(img) }
{
	m_graph.start = m_portals.start;
	m_graph.end = m_portals.end;
	m_graph.portals = m_portals;
}


void GraphBuilder::startExplore()
{
	m_currentTile = m_portals.start;

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

void GraphBuilder::startExploreRecursable()
{
	std::vector<Tile> startPoints = {m_portals.start, m_portals.end};
	for(const auto e : m_portals.hopIn)
	{
		startPoints.push_back(e.first);
		startPoints.push_back(e.second);
	}

	for(const auto & s : startPoints)
	{
		m_currentTile = s;

		if (m_graph.adjacencyList.find(m_currentTile) == m_graph.adjacencyList.end())
		{
			m_graph.adjacencyList[m_currentTile] = {};
		}

		if (m_visited.find(m_currentTile) == m_visited.end())
		{
			m_visited[m_currentTile] = {};
		}

		m_graph.tileMap[m_currentTile] = '.';

		exploreRecursable();
	}
}


bool GraphBuilder::advance(const Tile & t, Direction d)
{
	// std::cout << "Adv: (" << m_currentTile(0) << ", " << m_currentTile(1) << ") -> (" << t(0) << ", " << t(1) << ") [" << d <<
	// "]" << std::endl;

	char result = m_img.at(t);
	if (result != '.')
	{
		// std::cout << "\tWall" << std::endl;
		return false;
	}

	auto itVisited = m_visited.find(t);
	if (itVisited != m_visited.end())
	{
		auto itDirection = itVisited->second.find(d);
		if (itDirection != itVisited->second.end())
		{
			// std::cout << "\tAlready seen" << std::endl;
			return false;
		}
		else
		{
			itVisited->second.insert(d);
		}
	}
	else
	{
		m_visited[t] = { d };
	};

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

	m_currentTile = t;

	// std::cout << "\n\n" << m_graph.draw() << std::endl;

	return true;
}


void GraphBuilder::explore(Direction previous)
{
	for (int di = 1; di <= 4; ++di)
	{
		auto d = static_cast<Direction>(di);
		const auto t = nextTile(m_currentTile, d);

		if (advance(t, d))
		{
			explore(d);
		}
	}

	if(m_portals.canPortal(m_currentTile))
	{
		const auto d = Portal;
		const auto t = m_portals.portal(m_currentTile);
		if(advance(t, d))
		{
			explore(d);
		}
	}

	if (previous != Still)
	{
		if (previous == Portal)
		{
			m_currentTile = m_portals.portal(m_currentTile);
		}
		else
		{
			m_currentTile = nextTile(m_currentTile, inverse(previous));
		}
	}
}

void GraphBuilder::exploreRecursable(Direction previous)
{
	for (int di = 1; di <= 4; ++di)
	{
		auto d = static_cast<Direction>(di);
		const auto t = nextTile(m_currentTile, d);

		if (advance(t, d))
		{
			exploreRecursable(d);
		}
	}

	if (previous != Still)
	{
		m_currentTile = nextTile(m_currentTile, inverse(previous));
	}
}


Graph GraphBuilder::buildGraph(const Image & img)
{
	GraphBuilder b{ img };
	b.startExplore();
	return b.m_graph;
}

Graph GraphBuilder::buildRecursableGraph(const Image & img)
{
	GraphBuilder b{ img };
	b.startExploreRecursable();
	return b.m_graph;
}

DonutStructure::DonutStructure(const Image & img)
{
	row_outer_top = 2;
	row_outer_bottom = img.rows() - 3;

	col_outer_left = 2;
	col_outer_right = img.cols() - 3;

	const int row_center = img.rows() / 2 - 1;
	const int col_center = img.cols() / 2 - 1;

	for (int row = row_center; row > row_outer_top; --row)
	{
		const char c = img.at(row, col_center);
		if (c == '#' || c == '.')
		{
			row_inner_top = row;
			break;
		}
	}

	for (int row = row_center; row < row_outer_bottom; ++row)
	{
		const char c = img.at(row, col_center);
		if (c == '#' || c == '.')
		{
			row_inner_bottom = row;
			break;
		}
	}

	for (int col = col_center; col > col_outer_left; --col)
	{
		const char c = img.at(row_center, col);
		if (c == '#' || c == '.')
		{
			col_inner_left = col;
			break;
		}
	}

	for (int col = col_center; col < col_outer_right; ++col)
	{
		const char c = img.at(row_center, col);
		if (c == '#' || c == '.')
		{
			col_inner_right = col;
			break;
		}
	}
}


Portals findPortals(const Image & img)
{
	std::map<std::string, Portals::Portal> labels;

	const DonutStructure s{ img };

	auto tryAddTile = [&](const Tile & a, const Tile & b, const Tile & t, bool inner) {
		const auto ca = img.at(a);
		const auto cb = img.at(b);
		if (std::isupper(ca) && std::isupper(cb))
		{
			const auto id = std::string{ ca } + cb;
			if (labels.find(id) == labels.end())
			{
				labels[id] = {};
			}

			if (inner)
			{
				labels[id].innerPortal = t;
			}
			else
			{
				labels[id].outerPortal = t;
			}
		}
	};

	for (int col = s.col_outer_left; col < s.col_outer_right; ++col)
	{
		tryAddTile({ s.row_outer_top - 2, col }, { s.row_outer_top - 1, col }, { s.row_outer_top, col }, false);
	}

	for (int col = s.col_inner_left; col < s.col_inner_right; ++col)
	{
		tryAddTile({ s.row_inner_top + 1, col }, { s.row_inner_top + 2, col }, { s.row_inner_top, col }, true);
	}

	for (int col = s.col_inner_left; col < s.col_inner_right; ++col)
	{
		tryAddTile({ s.row_inner_bottom - 2, col }, { s.row_inner_bottom - 1, col }, { s.row_inner_bottom, col }, true);
	}

	for (int col = s.col_outer_left; col < s.col_outer_right; ++col)
	{
		tryAddTile({ s.row_outer_bottom + 1, col }, { s.row_outer_bottom + 2, col }, { s.row_outer_bottom, col }, false);
	}

	for (int row = s.row_outer_top; row < s.row_outer_bottom; ++row)
	{
		tryAddTile({ row, s.col_outer_left - 2 }, { row, s.col_outer_left - 1 }, { row, s.col_outer_left }, false);
	}

	for (int row = s.row_inner_top; row < s.row_inner_bottom; ++row)
	{
		tryAddTile({ row, s.col_inner_left + 1 }, { row, s.col_inner_left + 2 }, { row, s.col_inner_left }, true);
	}

	for (int row = s.row_inner_top; row < s.row_inner_bottom; ++row)
	{
		tryAddTile({ row, s.col_inner_right - 2 }, { row, s.col_inner_right - 1 }, { row, s.col_inner_right }, true);
	}

	for (int row = s.row_outer_top; row < s.row_outer_bottom; ++row)
	{
		tryAddTile({ row, s.col_outer_right + 1 }, { row, s.col_outer_right + 2 }, { row, s.col_outer_right }, false);
	}

	Portals ret;

	for (const auto & e : labels)
	{
		if (e.first == "AA")
		{
			ret.start = e.second.outerPortal;
		}
		else if (e.first == "ZZ")
		{
			ret.end = e.second.outerPortal;
		}
		else
		{
			ret.hopIn[e.second.innerPortal] = e.second.outerPortal;
			ret.hopOut[e.second.outerPortal] = e.second.innerPortal;
		}
	}

	return ret;
}

Portals::LevelChange Portals::canPortal(const Tile & t) const
{
	if(hopIn.find(t) != hopIn.end())
	{
		return In;
	}

	if(hopOut.find(t) != hopOut.end())
	{
		return Out;
	}

	return No;
}

Tile Portals::portal(const Tile & t) const
{
	switch (canPortal(t))
	{
		case In: return hopIn.at(t);
		case Out: return hopOut.at(t);
		case No: throw std::runtime_error("");
	}
}