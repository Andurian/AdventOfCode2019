#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "infix_ostream_iterator.h"
#include "read_file.h"

class Deck
{
public:
	Deck(int size)
	{
		m_cards.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			m_cards.push_back(i);
		}
	}

	void reverse() { std::reverse(m_cards.begin(), m_cards.end()); }

	void cut(int n)
	{
		if (n < 0)
		{
			n = m_cards.size() + n;
		}

		std::vector<int> temp;
		std::copy(m_cards.begin() + n, m_cards.end(), std::back_inserter(temp));
		std::copy(m_cards.begin(), m_cards.begin() + n, std::back_inserter(temp));
		m_cards = temp;
	}

	void deal(int n)
	{
		const auto s = m_cards.size();
		std::vector<int> temp(s, 0);
		for (size_t i = 0; i < s; ++i)
		{
			temp[(n * i) % s] = m_cards[i];
		}

		m_cards = temp;
	}

	void print() { printCommaSeparatedLine(m_cards); }

	size_t indexOf(int n)
	{
		auto it = std::find(m_cards.begin(), m_cards.end(), n);

		return it == m_cards.end() ? -1 : it - m_cards.begin();
	}

private:
	std::vector<int> m_cards;
};

class Action
{
public:
	Action(long long k, long long m) : m_k{ k }, m_m{ m }
	{
		// empty
	}

	long long operator()(long long x) const { return m_k * x + m_m; }

	void aggregate(Action a) { *this = Action{ m_k * a.m_k, a.m_k * m_m + a.m_m }; }

private:
	long long m_k;
	long long m_m;
};

std::vector<std::function<void(Deck &)>> parseFile(const std::string & filename)
{
	std::vector<std::string> commands = split_string(read_file(filename), "\n");

	std::vector<std::function<void(Deck &)>> ret;

	for (const auto & command : commands)
	{
		const auto tokens = split_string(command, " ");

		if (tokens[0] == "deal")
		{
			if (tokens[1] == "with")
			{
				ret.emplace_back([=](Deck & d) { d.deal(convert<int>(tokens.back())); });
			}
			else
			{
				ret.emplace_back([=](Deck & d) { d.reverse(); });
			}
		}
		else if (tokens[0] == "cut")
		{
			ret.emplace_back([=](Deck & d) { d.cut(convert<int>(tokens.back())); });
		}
	}

	return ret;
}

int main()
{
	const auto ops = parseFile("input.txt");

	Deck d{ 10007 };

	for (auto & op : ops)
	{
		op(d);
	}

	std::cout << "Index of 2019: " << d.indexOf(2019) << std::endl;

	d = Deck{ 10 };
	d.reverse();
	d.cut(2);
	d.cut(3);
	d.deal(3);
	d.cut(2);
	d.reverse();
	d.cut(3);
	d.deal(3);
	d.print();

	return 0;
}