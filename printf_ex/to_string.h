#pragma once

#include <string>
#include "format.h"

namespace PrintF_ex
{

	//
	// ToString - Helper functions to simplify formating
	//

	// /*
	// Converts a null-terminated wchar_t* string to a std::string
	inline std::string ToString(wchar_t const * value, size_t const size)
	{
		size_t n = 0;
		// duplicate the size to compensate for multibyte chars.
		auto const destSize = size * 2;
		std::string tmp(destSize, 'f');

		wcstombs_s(&n, &tmp[0], destSize, value, destSize - 1);
		tmp.shrink_to_fit();
		return tmp;
	}
	// */

	// Converts a null-terminated wchar_t * string to a std::string
	inline std::string ToString(wchar_t const * value)
	{
		return ToString(value, wcslen(value) + 1);
	}

	// /*
	// Converts a null-terminated char * string to a std::wstring
	inline std::wstring ToString(char const * value, size_t const size)
	{
		size_t n = 0;
		std::wstring tmp(size, L'f');
		
		mbstowcs_s(&n, &tmp[0], size, value, size - 1);
		tmp.shrink_to_fit();
		return tmp;
	}
	// */

	// Converts a null-terminated char * string to a std::wstring
	inline std::wstring ToString(char const * value)
	{
		return ToString(value, strlen(value) + 1);
	}

	// Returns a string representation of a given double or float value
	std::string ToString(float const value, unsigned const precision)
	{
		std::string result;
		Format(result, "%.*f", precision, value);
		return result;
	}

	// Returns a string representation of a given double or float value
	std::string ToString(double const value, unsigned const precision)
	{
		std::string result;
		Format(result, "%.*f", precision, value);
		return result;
	}
}