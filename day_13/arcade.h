#pragma once

#include <map>
#include <string>
#include <thread>

#include "console_image.h"

#include "Intprogram.h"
#include "InterProgramCommunication.h"

class Arcade
{
public:
	enum Tile
	{
		Empty,
		Wall,
		Block,
		Paddle,
		Ball
	};

	Arcade(int frameDelayMs);
	~Arcade();

	void run(const std::string & filename, bool insertQuarter);

	std::string getDisplayString() const;
	Integer getTileCount(Tile t) const;
	Integer getScore() const;

private:
	static void cls();

	void display() const;

	static const std::map<Tile, char> s_charMapping;

	InterProgramCommunication m_input;
	InterProgramCommunication m_output;

	std::thread m_programThread;
	std::thread m_playerThread;

	PointMap<char> m_content;
	Integer m_score;

	Integer m_ballPos;
	Integer m_pedalPos;

	int m_frameDelayMs;
};