#pragma once
#include <cstdint>
#include "Memory.h"
class CPU
{
public:

	// status flags

	struct StatusFlags
	{
		enum : std::uint8_t
		{
			None	= 0,
			OF		= 1 << 0,
			CF		= 1 << 1,
			ZF		= 1 << 2,
		};
	};

	std::uint8_t SF = StatusFlags::None;

	std::uint8_t MODE = 0;

	// interupts
	std::uint8_t SI = 0;
	std::uint8_t PI = 0;
	std::uint8_t TI = 100;

	std::uint8_t SB = 0;
	std::uint8_t DB = 0;
	std::uint8_t ST = 0;
	std::uint8_t DT = 0;


	std::uint32_t AX = 0;
	std::uint32_t BX = 0;
	std::uint32_t PTR = 0;

	std::uint16_t IC = 0;

public:

};

