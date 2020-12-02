#include "Parameter.h"

#include "Intprogram.h"


std::unique_ptr<InputParameter> InputParameter::make(ParameterMode mode, InstructionPointer offset)
{
	switch (mode)
	{
		case POSITION:
			return std::make_unique<PositionInputParameter>(offset);
		case IMMEDIATE:
			return std::make_unique<ImmediateInputParameter>(offset);
		case RELATIVE:
			return std::make_unique<RelativeInputParameter>(offset);
		default:
			throw std::runtime_error("Invalid Input Parameter Mode");
	}
}


InputParameter::InputParameter(InstructionPointer offset)
	: m_offset{ offset }
{
	// empty
}


PositionInputParameter::PositionInputParameter(InstructionPointer offset)
	: InputParameter{ offset }
{
	// empty
}


Integer PositionInputParameter::get(InstructionPointer pos, const Intprogram * program)
{
	return program->read(program->read(pos + m_offset));
}


ImmediateInputParameter::ImmediateInputParameter(InstructionPointer offset)
	: InputParameter{ offset }
{
	// empty
}


Integer ImmediateInputParameter::get(InstructionPointer pos, const Intprogram * program)
{
	return program->read(pos + m_offset);
}


RelativeInputParameter::RelativeInputParameter(InstructionPointer offset)
	: InputParameter{ offset }
{
	// empty
}


Integer RelativeInputParameter::get(InstructionPointer pos, const Intprogram * program)
{
	return program->readRelative(pos + m_offset);
}


std::unique_ptr<OutputParameter> OutputParameter::make(ParameterMode mode, InstructionPointer offset)
{
	switch (mode)
	{
		case POSITION:
			return std::make_unique<PositionOutputParameter>(offset);
		case IMMEDIATE:
			throw std::runtime_error("Immediate mode is invalid for output parameter.");
		case RELATIVE:
			return std::make_unique<RelativeOutputParameter>(offset);
		default:
			throw std::runtime_error("Invalid Input Parameter Mode");
	}
}


OutputParameter::OutputParameter(InstructionPointer offset)
	: m_offset{ offset }
{
	// empty
}


PositionOutputParameter::PositionOutputParameter(InstructionPointer offset)
	: OutputParameter{ offset }
{
	// empty
}


void PositionOutputParameter::write(InstructionPointer pos, Intprogram * program, Integer value)
{
	program->write(program->read(pos + m_offset), value);
}


RelativeOutputParameter::RelativeOutputParameter(InstructionPointer offset)
	: OutputParameter{ offset }
{
	// empty
}


void RelativeOutputParameter::write(InstructionPointer pos, Intprogram * program, Integer value)
{
	program->writeRelative(pos + m_offset, value);
}
