#pragma once
// Printf_ex internals

#include <cstdio>
#include <string>
#include "pfex_debug.h"
#include "printf_ex_types.h"

//#define __GNUG__ 3
//#undef _MSC_VER

namespace Red {
namespace details {

	template<typename ... Args>
	void internal_print(char const * format, EndL_t<char> const & endl, Args const & ... args)
	{
		std::string tmp(format);
		tmp += endl();

		printf(tmp.c_str(), PrintArg(args) ...);
	}

	template<typename ... Args>
	void internal_print(wchar_t const * format, EndL_t<wchar_t> const & endl, Args const & ... args)
	{
		std::wstring tmp(format);
		tmp += endl();

		wprintf(tmp.c_str(), PrintArg(args) ...);
	}

	template<typename ... Args>
	void internal_print(char const * format, Args const & ... args)
	{
		printf(format, PrintArg(args) ...);
	}

	template<typename ... Args>
	void internal_print(wchar_t const * format, Args const & ... args)
	{
		wprintf(format, PrintArg(args) ...);
	}

	template<typename ... Args>
	int internal_format_buffer(char * const buffer, size_t const bufferLen,
							   char const * const format, Args const & ... args) noexcept
	{
		return snprintf(buffer, bufferLen, format, PrintArg(args) ...);
	}

	template<typename ... Args>
	int internal_format_buffer(wchar_t * const buffer, size_t const bufferLen,
							   wchar_t const * const format, Args const & ... args) noexcept
	{
		return swprintf(buffer, bufferLen, format, PrintArg(args) ...);
	}

#ifdef _MSC_VER 
	template<typename ... Args>
	int get_required_size(wchar_t const * const format, Args const & ... args) noexcept
	{
		return _scwprintf(format, PrintArg(args)...);
	}

	template<typename ... Args>
	int get_required_size(char const * const format, Args const & ... args) noexcept
	{
		return _scprintf(format, PrintArg(args) ...);
	}
#else
	template<typename ... Args>
	int get_required_size(char const * const format, Args const & ... args) noexcept
	{
		return snprintf(nullptr, 0, format, PrintArg(args) ...);
	}

	// sadly, there is no standard wide char alternative to snprintf D:
	// brute force until swprintf spits out a number we can use
	template<typename ... Args>
	int get_required_size(wchar_t const * const format, Args const & ... args)
	{
		int result = 0;
		size_t i = 1, dummySize = 256, increment = 64;
		do
		{
			wchar_t * dummy = new wchar_t[dummySize];
			result = swprintf(dummy, dummySize, format, PrintArg(args)...);
			delete[] dummy;

			i++; dummySize += increment; if (i % 10 == 0) increment *= 2;
		} 
		while (result == -1);

		return result;
	}
#endif // _MSC_VER 


}
}
