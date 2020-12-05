#include <iostream>
#include <list>
#include <thread>

#include "InterProgramCommunication.h"
#include "Intprogram.h"
#include "enumerate.h"
#include "find_optional.h"
#include "read_file.h"

class NonBlockingIPC : public Input, public Output
{
public:
	NonBlockingIPC() = default;

	explicit NonBlockingIPC(Integer i) { m_queue.push(i); }

	Integer readNext() override
	{
		std::unique_lock<std::mutex> lock{ m_mtx };

		{
			std::unique_lock<std::mutex> statusLock{ m_statusMtx };
			m_isSomeoneWaiting = true;
		}

		if (!m_running)
		{
			throw Stopped{};
		}

		if (m_queue.empty())
		{
			return -1;
		}

		const auto result = m_queue.front();
		m_queue.pop();

		{
			std::unique_lock<std::mutex> statusLock{ m_statusMtx };
			m_isSomeoneWaiting = false;
		}

		return result;
	}

	void write(Integer value) override
	{
		std::unique_lock<std::mutex> lock(m_mtx);

		m_queue.push(value);
	}

	void write(Integer x, Integer y)
	{
		std::unique_lock<std::mutex> lock(m_mtx);

		m_queue.push(x);
		m_queue.push(y);
	}

	void stop()
	{
		std::unique_lock<std::mutex> lock(m_mtx);

		m_running = false;
	}

	bool isEmpty() const { return m_queue.empty(); }

	bool isSomeoneWaiting()
	{
		std::unique_lock<std::mutex> statusLock{ m_statusMtx };
		return m_isSomeoneWaiting;
	}


private:
	std::queue<Integer> m_queue;

	std::mutex m_mtx;
	std::mutex m_statusMtx;

	bool m_running = true;
	bool m_isSomeoneWaiting = false;
};

class NAT
{
public:
	std::optional<Integer> sendImpulse(NonBlockingIPC & ipc)
	{
		std::cout << "NAT -> 0: (" << m_x << ", " << m_y << ")" << std::endl;
		if (m_y == m_lastSentY)
		{
			return m_y;
		}

		ipc.write(m_x, m_y);
		m_lastSentY = m_y;
		return std::nullopt;
	}

	void write(Integer x, Integer y)
	{
		m_x = x;
		m_y = y;
	}

private:
	Integer m_x = 0, m_y = 0;
	Integer m_lastSentY = -1;
};

class Network
{
private:
	struct Package
	{
		Integer address, x, y;
	};

public:
	Network(const Code & code, int numComputers)
	{
		for (Integer i = 0; i < numComputers; ++i)
		{
			m_inputs.emplace_back(i);
			m_outputs.emplace_back();

			// Needs to be here. Otherwise strange data race...
			auto & in = m_inputs.back();
			auto & out = m_outputs.back();

			m_threads.emplace_back([&] { Intprogram{ in, out, code }.run(); });
		}
	}

	~Network()
	{
		for (auto & in : m_inputs)
		{
			in.stop();
		}

		for (auto & t : m_threads)
		{
			t.join();
		}
	}

	std::pair<Integer, Integer> routePackages()
	{
		int noPackageCnt = 0;
		while (true)
		{
			++noPackageCnt;
			for (auto [i, out] : enumerate(m_outputs))
			{
				if (auto package = tryReadPackage(out))
				{
					noPackageCnt = 0;
					sendPackage(*package, static_cast<int>(i));
				}
			}

			if(noPackageCnt > 10000)
			{
				noPackageCnt = 0;
				auto res = m_nat.sendImpulse(m_inputs.front());
				if(res)
				{
					return std::make_pair(*m_solution1, *res);
				}
			}
		}
	}

private:
	static std::optional<Package> tryReadPackage(NonBlockingIPC & out)
	{
		auto address = out.readNext();
		if (address == -1)
		{
			return std::nullopt;
		}

		auto x = out.readNext();
		while (x == -1)
		{
			x = out.readNext();
		}

		auto y = out.readNext();
		while (y == -1)
		{
			y = out.readNext();
		}

		return Package{ address, x, y };
	}

	void sendPackage(const Package & p, int source)
	{
		std::cout << source << " -> " << p.address << ": (" << p.x << ", " << p.y << ")" << std::endl;

		if (p.address == 255)
		{
			if (!m_solution1)
			{
				m_solution1 = p.y;
			}
			m_nat.write(p.x, p.y);
		}
		else
		{
			auto inputIt = m_inputs.begin();
			std::advance(inputIt, p.address);
			inputIt->write(p.x, p.y);
		}
	}

	// Apparently doesn't work
	bool isIdle(){
		for(auto & in : m_inputs)
		{
			if(!in.isSomeoneWaiting())
			{
				return false;
			}
		}

		for(auto & out: m_outputs)
		{
			if(!out.isEmpty())
			{
				return false;
			}
		}

		return true;
	}

	std::list<NonBlockingIPC> m_inputs;
	std::list<NonBlockingIPC> m_outputs;

	std::list<std::thread> m_threads;

	NAT m_nat;

	std::optional<Integer> m_solution1;
};


int main()
{
	const auto code = read_collection("input.txt", ",", Converter<Integer>{});
	auto res = Network{code, 50}.routePackages();
	std::cout << "Solution 1: " << res.first << std::endl;
	std::cout << "Solution 2: " << res.second << std::endl;

	return 0;
}