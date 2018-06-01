#pragma once
// Printf_ex internals

#include <cstdio>
#include <string>
#include "pfex_debug.h"


template<typename TVal>
TVal PrintArg(TVal value) noexcept
{
	return value;
}

template<typename Tchar>
Tchar const * PrintArg(std::basic_string<Tchar> const & value) noexcept
{
	return value.c_str();
}


namespace Red {
namespace details {

	template<typename ... Args>
	void internal_print(char const * format, Args const & ... args) noexcept
	{
		printf(format, PrintArg(args) ...);
	}

	template<typename ... Args>
	void internal_print(wchar_t const * format, Args const & ... args) noexcept
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

	template<typename ... Args>
	int get_required_size(wchar_t const * const format, Args const & ... args) noexcept
	{
		// open a noop FILE stream
	#ifdef _WIN32
		const char* null_dev = "nul";
	#else
		const char* null_dev = "/dev/null";
	#endif // _WIN32

		FILE * noop = fopen(null_dev, "w");
		int result = fwprintf(noop, format, PrintArg(args) ...);
		fclose(noop);

		return result * sizeof(wchar_t);
	}
#endif // _MSC_VER 


}
}
