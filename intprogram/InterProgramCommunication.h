#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Input.h"
#include "Output.h"


class InterProgramCommunication : public Input, public Output
{
public:
	InterProgramCommunication();
	InterProgramCommunication(std::initializer_list<Integer> values);
	~InterProgramCommunication() = default;

	Integer readNext() override;
	void write(Integer value) override;
	void stop();

private:
	std::queue<Integer> m_queue;

	std::mutex m_mtx;
	std::condition_variable m_condNotEmpty;

	bool m_running;
};
