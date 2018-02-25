#pragma once
// Part of: Printf_ex
// Copyright 2018-present Pedro Oliva Rodriges
// This code is released under the MIT licence

// ref: https://msdn.microsoft.com/magazine/dn913181

#include <cstdio>
#include <string>
#include <type_traits>
#include "debug.h"
#include "printtypes.h"


namespace PrintF_ex
{
	namespace details
	{

		template<typename ... Args>
		void _printing(char const * format, EndL_t<char> endl, Args const & ... args)
		{
			std::string tmp(format);
			tmp += endl.value;

			printf_s(tmp.c_str(), PrintArg(args) ...);
		}


		template<typename ... Args>
		void _printing(wchar_t const * format, EndL_t<wchar_t> endl, Args const & ... args)
		{
			std::wstring tmp(format);
			tmp += endl.value;

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
		int _printing_buffer(char * const buffer, size_t const bufferLen, char const * const format, Args const & ... args) noexcept
		{
			int const result = snprintf(buffer, bufferLen, format, PrintArg(args) ...);
			PF_ASSERT(-1 != result);
			return result;
		}

		template<typename ... Args>
		int _printing_buffer(wchar_t * const buffer, size_t const bufferLen, wchar_t const * const format, Args const & ... args) noexcept
		{
			int const result = _snwprintf(buffer, bufferLen, format, PrintArg(args) ...);
			PF_ASSERT(-1 != result);
			return result;
		}

	}

	//
	// Print - Wrappers around printf + some shortcuts
	//

	template <typename Tchar, class ... Args>
	void Print(Tchar const * format, Args const & ... args)
	{
		details::_printing(format, args ...);
	}

	template<typename Tchar, class ... Args>
	void Print(Tchar const * format, EndL_t<Tchar> endl, Args const & ... args)
	{
		details::_printing(format, endl, args ...);
	}

	inline void Print(char const * const value) noexcept
	{
		Print("%s", value);
	}

	inline void Print(char const * value, EndL_t<char> const endl)
	{
		details::_printing("%s", endl, value);
	}

	inline void Print(wchar_t const * const value) noexcept
	{
		Print(L"%s", value);
	}

	inline void Print(wchar_t const * value, EndL_t<wchar_t> const endl)
	{
		details::_printing(L"%s", endl, value);
	}


	template <typename T>
	void Print(std::basic_string<T> const & value) noexcept
	{
		Print(value.c_str());
	}

	template<class Tchar, class ... Args>
	void Print(std::basic_string<Tchar> const & format, Args const & ... args)
	{
		Print(format.c_str(), args ...);
	}

	template<class Tchar, class ... Args>
	void Print(std::basic_string<Tchar> const & format, EndL_t<Tchar> endl, 
			   Args const & ... args)
	{
		Print(format.c_str(), endl, args ...);
	}

	//
	// Printl - Same as Print, but adds a new line at the end
	//

	template <typename ... Args, size_t _FmtSize>
	void Printl(char const (&format)[_FmtSize], Args const & ... args) noexcept
	{
		char tmp[_FmtSize + 1];
		strcpy_s(tmp, format);
		strcat_s(tmp, "\n");

		details::_printing(tmp, args ...);
	}

	template <typename ... Args, size_t _FmtSize>
	void Printl(wchar_t const (&format)[_FmtSize], Args const & ... args) noexcept
	{
		wchar_t tmp[_FmtSize + 1];
		wcscpy_s(tmp, format);
		wcscat_s(tmp, L"\n");

		details::_printing(tmp, args ...);
	}

	template<class Tchar, class ... Args>
	void Printl(std::basic_string<Tchar> const & format, Args const & ... args)
	{
		if (std::is_same<Tchar, char>())
			details::_printing(format.c_str(), EndL_t<char>("\n"), args ...);
		else
			details::_printing(format.c_str(), EndL_t<wchar_t>(L"\n"), args ...);

	}

	inline void Printl(wchar_t const * const value) noexcept
	{
		Print(L"%s\n", value);
	}

	inline void Printl(char const * const value) noexcept
	{
		Print("%s\n", value);
	}

	template <typename T>
	void Printl(std::basic_string<T> const & value) noexcept
	{
		Printl(value.c_str());
	}

	//
	// PrintStr - Build formated text to a buffer
	// All overloads returns the N# of chars written
	//

	// Write a formated message to a buffer
	template <typename Tchar, typename ... Args>
	int PrintStr(Tchar * const buffer, 
				 size_t const bufferCount,
				 Tchar const * const format,
				 Args const & ... args) noexcept
	{
		return details::_printing_buffer(buffer, bufferCount, format, args ...);
	}

	// Write a formated message to a buffer
	template <typename Tchar, size_t _BufferSize, typename ... Args>
	int PrintStr(Tchar * const (&buffer)[_BufferSize],
				 Tchar const * const format,
				 Args const & ... args) noexcept
	{
		return details::_printing_buffer(buffer, _BufferSize, format, args ...);
	}

	// Write a formated message to a string
	template <typename Tchar, typename ... Args>
	int PrintStr(std::basic_string<Tchar> & buffer,
				 Tchar const * const format,
				 Args const & ... args)
	{
		size_t const size = PrintStr(&buffer[0],
									 buffer.size() + 1,
									 format,
									 args ...);
		// resize if needed
		if (size > buffer.size())
		{
			buffer.resize(size);
			PrintStr(&buffer[0], buffer.size() + 1, format, args ...);
		}
		else if (size < buffer.size())
		{
			buffer.resize(size);
		}

		return size;
	}

	//
	// ToString - Helper functions to simplify formating
	//

	// Converts a null-terminated wchar_t* string to a std::string
	inline std::string ToString(wchar_t const * value, size_t const size)
	{
		size_t n = 0;
		// duplicate the size to compensate for multibyte chars.
		auto const destSize = size * 2;
		std::string tmp(destSize, 'f');
		wcstombs_s(&n, &tmp[0], destSize, value, destSize - 1);
		tmp.resize(n);
		return tmp;
	}

	// Converts a null-terminated wchar_t * string to a std::string
	inline std::string ToString(wchar_t const * value)
	{
		return ToString(value, wcslen(value) + 1);
	}

	// Converts a null-terminated char * string to a std::wstring
	inline std::wstring ToString(char const * value, size_t const size)
	{
		size_t n = 0;
		std::wstring tmp(size, L'f');
		mbstowcs_s(&n, &tmp[0], size, value, size - 1);
		tmp.resize(n);
		return tmp;
	}

	// Converts a null-terminated char * string to a std::wstring
	inline std::wstring ToString(char const * value)
	{
		return ToString(value, strlen(value) + 1);
	}

	// Returns a string representation of a given double or float value
	std::string ToString(float const value, unsigned const precision)
	{
		std::string result;
		PrintStr(result, "%.*f", precision, value);
		return result;
	}

	// Returns a string representation of a given double or float value
	std::string ToString(double const value, unsigned const precision)
	{
		std::string result;
		PrintStr(result, "%.*f", precision, value);
		return result;
	}
}

