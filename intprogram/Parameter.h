#pragma once

#include <memory>

#include "int_types.h"

// forward declaration
class Intprogram;


enum ParameterMode
{
	POSITION = 0,
	IMMEDIATE = 1,
	RELATIVE = 2
};


class InputParameter
{
public:
	InputParameter(InstructionPointer offset);
	virtual ~InputParameter() = default;

	virtual Integer get(InstructionPointer pos, const Intprogram * program) = 0;

	static std::unique_ptr<InputParameter> make(ParameterMode mode, InstructionPointer offset);

protected:
	InstructionPointer m_offset;
};


class PositionInputParameter : public InputParameter
{
public:
	PositionInputParameter(InstructionPointer offset);
	~PositionInputParameter() = default;

	Integer get(InstructionPointer pos, const Intprogram * program) override;
};


class ImmediateInputParameter : public InputParameter
{
public:
	ImmediateInputParameter(InstructionPointer offset);
	~ImmediateInputParameter() = default;

	Integer get(InstructionPointer pos, const Intprogram * program) override;
};


class RelativeInputParameter : public InputParameter
{
public:
	RelativeInputParameter(InstructionPointer offset);
	~RelativeInputParameter() = default;

	Integer get(InstructionPointer pos, const Intprogram * program) override;
};


class OutputParameter
{
public:
	OutputParameter(InstructionPointer offset);
	virtual ~OutputParameter() = default;

	virtual void write(InstructionPointer pos, Intprogram * program, Integer value) = 0;

	static std::unique_ptr<OutputParameter> make(ParameterMode mode, InstructionPointer offset);

protected:
	InstructionPointer m_offset;
};


class PositionOutputParameter : public OutputParameter
{
public:
	PositionOutputParameter(InstructionPointer offset);
	~PositionOutputParameter() = default;

	void write(InstructionPointer pos, Intprogram * program, Integer value) override;
};


class RelativeOutputParameter : public OutputParameter
{
public:
	RelativeOutputParameter(InstructionPointer offset);
	~RelativeOutputParameter() = default;

	void write(InstructionPointer pos, Intprogram * program, Integer value) override;
};
