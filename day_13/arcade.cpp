#include "arcade.h"

#include <iostream>

#include "read_file.h"

const std::map<Arcade::Tile, char> Arcade::s_charMapping{
	{ Arcade::Empty, ' ' },
	{ Arcade::Wall, '/' },
	{ Arcade::Block, '#' },
	{ Arcade::Paddle, '=' },
	{ Arcade::Ball, 'o' },
};


Arcade::Arcade()
	: m_score{ 0 }
{
	// empty
}


Arcade::~Arcade()
{
	if (m_programThread.joinable())
	{
		m_programThread.join();
	}

	if(m_playerThread.joinable())
	{
		m_playerThread.join();
	}

	//system("cls");
	std::cout << "Score: " << m_score << "\n" << generate_image(m_content, ' ') << std::endl << std::endl;
}


void Arcade::run(const std::string & filename)
{
	m_programThread = std::thread{
		[this, &filename]
		{
			Intprogram p{ m_input, m_output, read_collection(filename, ",", Converter<Integer>{ }) };
			//p.write(0, 2);
			p.run();
			m_input.stop();
			m_output.stop();
		}
	};

	m_playerThread = std::thread{
		[this]
		{
			while (true)
			{
				try
				{
					auto posX = m_output.readNext();
					auto posY = m_output.readNext();
					auto info = m_output.readNext();

					if(posX == -1)
					{
						m_score = info;
						continue;
					}

					const auto tile = static_cast<Tile>(info);
					m_content[{ posX, posY }] = s_charMapping.at(tile);

					if(tile == Paddle)
					{
						m_pedalPos = posX;
					}

					if(tile == Ball)
					{
						m_ballPos = posX;

						if(m_ballPos > m_pedalPos)
						{
							m_input.write(1);
						}
						else if(m_ballPos < m_pedalPos)
						{
							m_input.write(-1);
						}
						else
						{
							m_input.write(0);
						}

						//system("cls");
						std::cout << "Score: " << m_score << "\n" << generate_image(m_content, ' ') << std::endl << std::endl;
					}
				}
				catch (const InterProgramCommunication::Stopped &)
				{
					std::cout << "Program stopped" << std::endl;
					break;
				}
			}
		}
	};
}


Integer Arcade::getCount(Tile t)
{
	return std::count_if(
		m_content.begin(), m_content.end(), [this, t](const std::pair<Point, char> & p)
		{
			return p.second == s_charMapping.at(t);
		});
}
