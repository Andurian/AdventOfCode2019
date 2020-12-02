#include "ascii.h"

#include "console_image.h"
#include "read_file.h"

#include <iostream>


Image::Image(std::vector<char> data, int width)
	: m_data{ std::move(data) }
	, m_width{ width }
{
	m_height = static_cast<int>(m_data.size()) / m_width;
	if (m_height * m_width != m_data.size())
	{
		throw std::runtime_error("");
	}
}


char Image::at(int row, int col) const
{
	return m_data.at(row * m_width + col);
}


char & Image::at(int row, int col)
{
	return m_data.at(row * m_width + col);
}


std::string Image::draw() const
{
	std::stringstream ss;

	for (int col = 0; col < m_width; ++col)
	{
		ss << at(0, col);
	}

	for (int row = 1; row < m_height; ++row)
	{
		ss << "\n";
		for (int col = 0; col < m_width; ++col)
		{
			ss << at(row, col);
		}
	}
	return ss.str();
}


Image fromProgram(std::vector<Integer> code)
{
	InterProgramCommunication in, out;
	std::thread t{
		[&]
		{
			Intprogram{ in, out, code }.run();
			in.stop();
			out.stop();
		}
	};

	int width = 0;
	std::vector<char> img;

	while (true)
	{
		try
		{
			const auto val = out.readNext();
			if (val == 10)
			{
				if (width == 0)
				{
					width = static_cast<int>(img.size());
				}
			}
			else
			{
				img.push_back(static_cast<char>(val));
			}
		}
		catch (const Input::Stopped &)
		{
			break;
		}
	}

	t.join();
	return Image{ img, width };
}


std::vector<std::tuple<int, int>> getIntersections(const Image & img)
{
	std::vector<std::tuple<int, int>> ret;
	for (int row = 1; row < img.rows() - 1; ++row)
	{
		for (int col = 1; col < img.cols() - 1; ++col)
		{
			const bool is = img.at(row, col) == '#' && img.at(row, col - 1) == '#' && img.at(row, col + 1) == '#'
			                && img.at(row - 1, col) == '#' && img.at(row + 1, col) == '#';
			if (is)
			{
				ret.push_back(std::make_tuple(row, col));
			}
		}
	}
	return ret;
}


Image markIntersections(Image img, const std::vector<std::tuple<int, int>> & inter)
{
	for (const auto & i : inter)
	{
		img.at(std::get<0>(i), std::get<1>(i)) = 'O';
	}
	return img;
}


int getAlignmentParameter(const Image & img)
{
	const auto inter = getIntersections(img);
	int ret = 0;
	for (const auto & i : inter)
	{
		ret += std::get<0>(i) * std::get<1>(i);
	}
	return ret;
}


std::string PathBuilder::buildPath(const Image & image)
{
	PathBuilder b{ image };

	for (int r = 0; r < image.rows(); ++r)
	{
		for (int c = 0; c < image.cols(); ++c)
		{
			const auto px = image.at(r, c);
			if (px == '.' || px == '#')
			{
				continue;
			}

			Direction d;
			switch (px)
			{
				case '^':
					d = North;
					break;
				case '<':
					d = West;
					break;
				case 'v':
					d = South;
					break;
				case '>':
					d = East;
			}

			b.build(Tile{ r, c }, d);
			return b.m_path;
		}
	}

	return "";
}


namespace
{
	Tile nextTile(Tile t, Direction d)
	{
		switch (d)
		{
			case North:
				return t + Tile{ -1, 0 };
			case South:
				return t + Tile{ 1, 0 };
			case West:
				return t + Tile{ 0, -1 };
			case East:
				return t + Tile{ 0, 1 };
			default:
				throw std::runtime_error("");
		}
	}


	Direction nextDirectionRight(Direction d)
	{
		switch (d)
		{
			case North:
				return East;
			case South:
				return West;
			case West:
				return North;
			case East:
				return South;
			default:
				throw std::runtime_error("");
		}
	}


	Direction nextDirectionLeft(Direction d)
	{
		switch (d)
		{
			case North:
				return West;
			case South:
				return East;
			case West:
				return South;
			case East:
				return North;
			default:
				throw std::runtime_error("");
		}
	}
} // namespace


PathBuilder::PathBuilder(const Image & img)
	: m_img{ img }
{
	// empty
}


void PathBuilder::build(const Tile & start, Direction d)
{
	m_currentPos = start;
	m_dir = d;

	while (advance()) { }

	std::stringstream ss;
	int fcount = 0;
	bool first = true;
	for (const char c : m_path)
	{
		if (c == 'F')
		{
			++fcount;
			continue;
		}

		if (fcount != 0)
		{
			if (!first)
			{
				ss << ",";
			}
			else
			{
				first = false;
			}
			ss << fcount;
			fcount = 0;
		}

		if (!first)
		{
			ss << ",";
		}
		else
		{
			first = false;
		}
		ss << c;
	}

	if (fcount != 0)
	{
		if (!first)
		{
			ss << ",";
		}
		ss << fcount;
	}

	m_path = ss.str();
}


bool PathBuilder::advance()
{
	Tile t = nextTile(m_currentPos, m_dir);
	if (m_img.isInside(t) && m_img.at(t) == '#')
	{
		m_path += "F";
		m_currentPos = t;
		return true;
	}

	t = nextTile(m_currentPos, nextDirectionLeft(m_dir));
	if (m_img.isInside(t) && m_img.at(t) == '#')
	{
		m_path += "L";
		m_dir = nextDirectionLeft(m_dir);
		return true;
	}

	t = nextTile(m_currentPos, nextDirectionRight(m_dir));
	if (m_img.isInside(t) && m_img.at(t) == '#')
	{
		m_path += "R";
		m_dir = nextDirectionRight(m_dir);
		return true;
	}

	return false;
}

void writeLine(const std::string & s, InterProgramCommunication & p)
{
	for(const auto c : s)
	{
		p.write(static_cast<Integer>(c));
	}
	p.write(10);
}

Integer dust(const std::vector<Integer> & code)
{
	// TODO: Write algorithm to automatically compute the routine.
	InterProgramCommunication in, out;
	std::string path = "A,B,B,A,C,A,C,A,C,B";
	std::string a = "R,6,R,6,R,8,L,10,L,4";
	std::string b = "R,6,L,10,R,8";
	std::string c = "L,4,L,12,R,6,L,10";

	writeLine(path, in);
	writeLine(a, in);
	writeLine(b, in);
	writeLine(c, in);
	in.write(static_cast<Integer>('n'));
	in.write(10);

	std::thread t{
		[&]
		{
			Intprogram p{ in, out, code };
			p.write(0, 2);
			p.run();
			in.stop();
			out.stop();
		}
	};

	Integer dst = 0;

	while(true)
	{
		try
		{
			const auto v = out.readNext();
			//std::cout << v << std::endl;
			dst = v;
		}
		catch(const Input::Stopped &)
		{
			break; 
		}
	}

	t.join();

	return dst;
}
