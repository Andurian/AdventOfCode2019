#include "arcade.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "read_file.h"

const std::map<Arcade::Tile, char> Arcade::s_charMapping{
	{ Arcade::Empty, ' ' }, { Arcade::Wall, '/' }, { Arcade::Block, '#' }, { Arcade::Paddle, '=' }, { Arcade::Ball, 'o' },
};


Arcade::Arcade(int frameDelayMs) : m_score{ 0 }, m_frameDelayMs{ frameDelayMs }
{
	// empty
}


Arcade::~Arcade()
{
	if (m_programThread.joinable())
	{
		m_programThread.join();
	}

	if (m_playerThread.joinable())
	{
		m_playerThread.join();
	}
}


void Arcade::run(const std::string & filename, bool insertQuarter)
{
	m_programThread = std::thread{ [this, &filename, insertQuarter] {
		Intprogram p{ m_input, m_output, read_collection(filename, ",", Converter<Integer>{}) };

		if (insertQuarter)
		{
			p.write(0, 2);
		}

		p.run();

		m_input.stop();
		m_output.stop();
	} };

	m_playerThread = std::thread{ [this] {
		while (true)
		{
			try
			{
				auto posX = m_output.readNext();
				auto posY = m_output.readNext();
				auto info = m_output.readNext();

				if (posX == -1)
				{
					m_score = info;
					continue;
				}

				const auto tile = static_cast<Tile>(info);
				m_content[{ posX, posY }] = s_charMapping.at(tile);

				if (tile == Paddle)
				{
					m_pedalPos = posX;
				}

				if (tile == Ball)
				{
					m_ballPos = posX;

					if (m_ballPos > m_pedalPos)
					{
						m_input.write(1);
					}
					else if (m_ballPos < m_pedalPos)
					{
						m_input.write(-1);
					}
					else
					{
						m_input.write(0);
					}

					display();
				}
			}
			catch (const InterProgramCommunication::Stopped &)
			{
				display();
				break;
			}
		}
	} };

	m_programThread.join();
	m_playerThread.join();
}


Integer Arcade::getTileCount(Tile t) const
{
	return std::count_if(m_content.begin(), m_content.end(), [this, t](const std::pair<Point, char> & p) {
		return p.second == s_charMapping.at(t);
	});
}

void Arcade::cls()
{
#ifdef WINDOWS
	std::system("cls");
#else
	std::system("clear");
#endif
}

std::string Arcade::getDisplayString() const
{
	std::stringstream ss;
	ss << "Score: " << m_score << "\n" << generate_image(m_content, ' ');
	return ss.str();
}

void Arcade::display() const
{
	if(m_frameDelayMs > 0)
	{
		cls();
		std::cout << getDisplayString() << std::endl << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds{ m_frameDelayMs });
	}
}

Integer Arcade::getScore() const
{
	return m_score;
}
