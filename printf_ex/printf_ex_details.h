#pragma once
// Printf_ex internals

#include <cstdio>
#include <stdexcept>
#include <string>
#include "debug.h"
#include "printf_ex_types.h"


namespace Red {
namespace details {

	template<typename ... Args>
	void _printing(char const * format, EndL_t<char> endl, Args const & ... args)
	{
		std::string tmp(format);
		tmp += endl();

		printf_s(tmp.c_str(), PrintArg(args) ...);
	}

	template<typename ... Args>
	void _printing(wchar_t const * format, EndL_t<wchar_t> endl, Args const & ... args)
	{
		std::wstring tmp(format);
		tmp += endl();

		wprintf_s(tmp.c_str(), PrintArg(args) ...);
	}

	template<typename ... Args>
	void _printing(char const * format, Args const & ... args)
	{
		printf_s(format, PrintArg(args) ...);
	}

	template<typename ... Args>
	void _printing(wchar_t const * format, Args const & ... args)
	{
		wprintf_s(format, PrintArg(args) ...);
	}

	template<typename ... Args>
	int unsafe_format_buffer(char * const buffer, size_t const bufferLen,
							 char const * const format, Args const & ... args) noexcept
	{
		return snprintf(buffer, bufferLen, format, PrintArg(args) ...);
	}

	template<typename ... Args>
	int unsafe_format_buffer(wchar_t * const buffer, size_t const bufferLen,
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

	// sadly, there is no standard wide char alternative to snprintf for wide chars, so I made my own
	template<typename ... Args>
	int get_required_size(wchar_t const * const format, Args const & ... args)
	{
		int result = -1;

		for (size_t i = 1, dummySize = 256, increment = 64;
			 result == -1;
			 dummySize += increment)
		{
			if (i % 10 == 0)
			{
				increment *= 2;
			}

			wchar_t * dummy = new wchar_t[dummySize];
			result = swprintf(dummy, dummySize, format, PrintArg(args) ...);
			delete[] dummy;
		}

		return result;
	}
#endif // _MSC_VER 



	inline void ensure_valid_fmt_result(int fmtResult)
	{
		PF_ASSERT(fmtResult != -1);
		if (fmtResult == -1)
		{
			throw std::runtime_error("Failed to format buffer, check your arguments.");
		}
	}

}
}
