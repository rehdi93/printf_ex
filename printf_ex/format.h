#pragma once

#include <cstdio>
#include "debug.h"

namespace PrintF_ex
{
	//
	// Format - Build formated text to a buffer
	//

	// Write a formated message to a string
	template <typename ... Args>
	int Format(char * const buffer,
			   size_t const bufferCount,
			   char const * const format,
			   Args const & ... args) noexcept
	{
		int const result = snprintf(buffer,
									bufferCount,
									format,
									Printable(args) ...);
		ASSERT(-1 != result);
		return result;
	}

	// Write a formated message to a string
	template <typename ... Args, size_t _BufferSize>
	int Format(char * const (&buffer)[_BufferSize],
			   char const * const format,
			   Args const & ... args) noexcept
	{
		return Format(buffer, _BufferSize, format, args ...);
	}

	// Write a formated message to a string
	template <typename ... Args>
	int Format(wchar_t * const buffer,
			   size_t const bufferCount,
			   wchar_t const * const format,
			   Args const & ... args) noexcept
	{
		int const result = swprintf_s(buffer,
									  bufferCount,
									  format,
									  Printable(args) ...);
		ASSERT(-1 != result);
		return result;
	}

	// Write a formated message to a string
	template <typename ... Args, size_t _BufferSize>
	int Format(wchar_t const (&buffer)[_BufferSize],
			   wchar_t const * const format,
			   Args const & ... args) noexcept
	{
		return Format(buffer, _BufferSize, format, args ...);
	}

	// Write a formated message to a string
	template <typename T, typename ... Args>
	void Format(std::basic_string<T> & buffer,
				T const * const format,
				Args const & ... args)
	{
		size_t const size = Format(&buffer[0],
								   buffer.size() + 1,
								   format,
								   args ...);
		if (size > buffer.size())
		{
			buffer.resize(size);
			Format(&buffer[0], buffer.size() + 1, format, args ...);
		}
		else if (size < buffer.size())
		{
			buffer.resize(size);
		}
	}
}