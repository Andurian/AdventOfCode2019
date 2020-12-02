#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

using Integer = long long;

class Contingent
{
public:
	Contingent(std::unordered_map<std::string, Integer> initialValues);

	bool has(const std::string & name, Integer quantity) const;
	Integer amountOf(const std::string & name) const;

	void remove(const std::string & name, Integer quantity);
	void add(const std::string & name, Integer quantity);

	auto size() const { return m_materials.size(); }

	friend std::ostream & operator<<(std::ostream & os, const Contingent & c);

private:
	std::unordered_map<std::string, Integer> m_materials;
};


std::ostream & operator<<(std::ostream & os, const Contingent & c);