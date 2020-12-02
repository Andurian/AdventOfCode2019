#include "InterProgramCommunication.h"

InterProgramCommunication::InterProgramCommunication() : m_running{true}
{
	// empty
}

InterProgramCommunication::InterProgramCommunication(std::initializer_list<Integer> values)
	: m_queue{ values }
	, m_running{ true }
{
	// empty
}


void InterProgramCommunication::write(Integer value)
{
	std::unique_lock<std::mutex> lock(m_mtx);

	m_queue.push(value);
	m_condNotEmpty.notify_all();
}


void InterProgramCommunication::stop()
{
	std::unique_lock<std::mutex> lock(m_mtx);

	m_running = false;
	m_condNotEmpty.notify_all();
}


Integer InterProgramCommunication::readNext()
{
	std::unique_lock<std::mutex> lock{ m_mtx };
	m_condNotEmpty.wait(lock, [&]() { return !m_queue.empty() || !m_running; });

	if(m_queue.empty() && !m_running)
	{
		throw Stopped{};
	}

	const auto result = std::move(m_queue.front());
	m_queue.pop();
	return result;
}

