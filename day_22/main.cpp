#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include <boost/range/adaptor/reversed.hpp>

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

long long multiplicativeInverse(long long a, long long n)
{
	long long t = 0;
	long long newt = 1;
	long long r = n;
	long long newr = a;

	while (newr != 0)
	{
		auto quotient = r / newr;
		t -= quotient * newt;
		r -= quotient * newr;
		std::swap(t, newt);
		std::swap(r, newr);
	}

	if (r > 1)
	{
		throw std::runtime_error("invalid n");
	}
	while (t < 0)
	{
		t += n;
	}

	return t;
}

long long addmod(long long x, long long y, long long m)
{
	x %= m;
	y %= m;
	long long sum = x - m + y; // -m <= sum < m-1
	return sum < 0 ? sum + m : sum;
}

long long timesmod(long long x, long long y, long long m)
{
	x %= m;
	y %= m;
	auto a = x < y ? x : y; // min
	auto b = x < y ? y : x; // max
	long long product = 0;
	for (; a != 0; a >>= 1, b = addmod(b, b, m))
		if (a & 1)
			product = addmod(product, b, m);
	return product;
}

/// Gets values such that f(p) = p * res.0 + res.1 is a function
/// where p is a position of a card in the deck, and f(p) is the
/// position of that card before shuffling the deck once.
std::pair<long long, long long> get_mul_add_to_reverse_shuffle(const std::vector<std::string> & commands, long long deckSize)
{
	long long mul = 1;
	long long add = 0;

	for (const auto & s : boost::adaptors::reverse(commands))
	{
		const auto tokens = split_string(s, " ");

		if (tokens[0] == "deal")
		{
			if (tokens[1] == "with")
			{
				auto increment = convert<int>(tokens.back());
				auto x = multiplicativeInverse(increment, deckSize);
				mul = timesmod(mul, x, deckSize);
				add = timesmod(add, x, deckSize);
			}
			else
			{
				add += 1;
				auto x = deckSize - 1;
				mul = timesmod(mul, x, deckSize);
				add = timesmod(add, x, deckSize);
			}
		}
		else if (tokens[0] == "cut")
		{
			auto amount = convert<int>(tokens.back());
			add += amount < 0 ? deckSize + amount : amount;
			add %= deckSize;
		}
	}
	return std::make_pair(mul, add);
}

unsigned long long modular_pow(unsigned long long base, unsigned long long exp, unsigned long long modulus)
{
	if (modulus == 1)
	{
		return 0;
	}

	unsigned long long res = 1;
	base %= modulus;
	while (exp > 0)
	{
		if ((exp % 2) == 1)
		{
			res = timesmod(res, base, modulus);
		}
		exp >>= 1;
		base = timesmod(base, base, modulus);
	}

	return res;
}


long long nrInPositionAfter(const std::vector<std::string> & commands, long long position, long long deckSize, long long repCnt)
{
	auto [mul, add] = get_mul_add_to_reverse_shuffle(commands, deckSize);

	// Explanation:
	// m = multiplier
	// a = addition
	// f(0) = p + 0
	// f(1) = (p) * m + a = pm + a
	// f(2) = (pm + a) * m + a = pm^2 + am + a
	// f(3) = (pm^2 + am + a) * m + a = pm^3 + am^2 + am + a
	// f(4) = (pm^3 + am^2 + am + a) * m + a = pm^4 + am^3 + am^2 + am + a
	//
	// It can also be rewritten as:
	// f(x) = pm^x + g(x)
	// g(0) = 0
	// g(x) = mg(x - 1) + a
	// Where g is a linear non-homogenous recurrence, which can be rewritten as:
	// g(x) = (am^x - a) / (m - 1)
	//
	// Consequently, calculating all repetitions can be done using:
	// f(x) = pm^x + (am^x - a) / (m - 1)

	long long mx = modular_pow(static_cast<unsigned long long>(mul), repCnt, deckSize);
	auto pmx = timesmod(position, mx, deckSize);
	auto amx = timesmod(add, mx, deckSize);
	auto inv = multiplicativeInverse(mul - 1, deckSize);
	auto res = addmod(pmx, timesmod(amx - add, inv, deckSize), deckSize);
	if (res < 0)
	{
		return res + deckSize;
	}
	else
	{
		return res;
	}
}

int part1(const std::string & filename)
{
	const auto ops = parseFile("input.txt");

	Deck d{ 10007 };

	for (auto & op : ops)
	{
		op(d);
	}

	return d.indexOf(2019);
}

long long part2(const std::string & filename)
{
	// Solution more or less copied from https://github.com/Aidiakapi/advent_of_code_2019/blob/master/src/day22.rs
	// I don't really have an idea what's going on...
	std::vector<std::string> commands = split_string(read_file(filename), "\n");
	return nrInPositionAfter(commands, 2020, 119315717514047, 101741582076661);
}

int main()
{
	std::cout << "Index of 2019:  " << part1("input.txt") << std::endl;
	std::cout << "Nr in pos 2020: " << part2("input.txt") << std::endl;

	return 0;
}