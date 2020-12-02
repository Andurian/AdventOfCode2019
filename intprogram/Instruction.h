#pragma once

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "int_types.h"
#include "Parameter.h"

// forward declaration
class Intprogram;


class Instruction
{
public:
	enum Opcode
	{
		ADD = 1,
		MUL = 2,
		INPUT = 3,
		OUTPUT = 4,
		JUMP_IF_TRUE = 5,
		JUMP_IF_FALSE = 6,
		LESS_THAN = 7,
		EQUALS = 8,
		ADJUST_BASE = 9,
		END = 99
	};


	struct Identifier
	{
		Identifier(Integer i); // implicit

		Opcode opcode;
		ParameterMode first, second, third;
	};


	using InputParameterPtr = std::unique_ptr<InputParameter>;
	using OutputParameterPtr = std::unique_ptr<OutputParameter>;

	static std::unique_ptr<Instruction> makeInstruction(Identifier id);

	virtual ~Instruction() = default;

	virtual InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) = 0;
};


class InstructionAdd : public Instruction
{
public:
	InstructionAdd(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput);
	~InstructionAdd() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
	OutputParameterPtr m_writeOutput;
};


class InstructionMul : public Instruction
{
public:
	InstructionMul(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput);
	~InstructionMul() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
	OutputParameterPtr m_writeOutput;
};


class InstructionInput : public Instruction
{
public:
	InstructionInput(OutputParameterPtr writeOutput);
	~InstructionInput() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;
private:
	OutputParameterPtr m_writeOutput;
};


class InstructionOutput : public Instruction
{
public:
	explicit InstructionOutput(InputParameterPtr getInput);
	~InstructionOutput() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput;
};


class InstructionJumpIfTrue : public Instruction
{
public:
	InstructionJumpIfTrue(InputParameterPtr getInput1, InputParameterPtr getInput2);
	~InstructionJumpIfTrue() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
};


class InstructionJumpIfFalse : public Instruction
{
public:
	InstructionJumpIfFalse(InputParameterPtr getInput1, InputParameterPtr getInput2);
	~InstructionJumpIfFalse() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
};


class InstructionLessThan : public Instruction
{
public:
	InstructionLessThan(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput);
	~InstructionLessThan() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
	OutputParameterPtr m_writeOutput;
};


class InstructionEquals : public Instruction
{
public:
	InstructionEquals(InputParameterPtr getInput1, InputParameterPtr getInput2, OutputParameterPtr writeOutput);
	~InstructionEquals() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput1, m_getInput2;
	OutputParameterPtr m_writeOutput;
};


class InstructionAdjustBase : public Instruction
{
public:
	InstructionAdjustBase(InputParameterPtr getInput);
	~InstructionAdjustBase() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;

private:
	InputParameterPtr m_getInput;
};


class InstructionEnd : public Instruction
{
public:
	InstructionEnd() = default;
	~InstructionEnd() = default;

	InstructionPointer execute(InstructionPointer instructionPointer, Intprogram * program) override;
};
