#pragma once
// Printf_ex main header
// made by: Pedro Oliva Rodriges

// ref: https://msdn.microsoft.com/magazine/dn913181

#include <cstdio>
#include <string>
#include <type_traits>

#ifdef __GNUG__
#include <locale>
#endif // __GNUG__

#include "printf_ex_details.h"
#include "debug.h"


namespace Red
{

	//
	// Print - Wrappers around printf + some shortcuts
	//

	template <typename Tchar, class ... Args>
	void Print(Tchar const * format, Args const & ... args) noexcept
	{
		details::_printing(format, args ...);
	}

	template<typename Tchar, class ... Args>
	void Print(Tchar const * format, EndL_t<Tchar> const & endl, Args const & ... args) noexcept
	{
		details::_printing(format, endl, args ...);
	}

	inline void Print(char const * const value) noexcept
	{
		Print("%s", value);
	}

	inline void Print(char const * value, EndL_t<char> const & endl) noexcept
	{
		Print("%s", endl, value);
	}

	inline void Print(wchar_t const * const value) noexcept
	{
		Print(L"%ls", value);
	}

	inline void Print(wchar_t const * value, EndL_t<wchar_t> const & endl) noexcept
	{
		Print(L"%ls", endl, value);
	}


	template <typename T>
	void Print(std::basic_string<T> const & value) noexcept
	{
		Print(value.c_str());
	}

	template<class Tchar, class ... Args>
	void Print(std::basic_string<Tchar> const & format, Args const & ... args) noexcept
	{
		Print(format.c_str(), args ...);
	}

	template<class Tchar, class ... Args>
	void Print(std::basic_string<Tchar> const & format, EndL_t<Tchar> const & endl,
			   Args const & ... args) noexcept
	{
		Print(format.c_str(), endl, args ...);
	}

	//
	// Printl - Shorthand for writing a message and adding a new line at the end
	//

	template <typename ... Args>
	void Printl(char const * format, Args const & ... args) noexcept
	{
		//char tmp[_FmtSize + 2];
		//strcpy_s(tmp, format);
		//strcat_s(tmp, "\n");
		//details::_printing(tmp, args ...);

		Print(format, args...); Print("\n");
	}

	template <typename ... Args>
	void Printl(wchar_t const * format, Args const & ... args) noexcept
	{
		/*wchar_t tmp[_FmtSize + 2];
		wcscpy_s(tmp, format);
		wcscat_s(tmp, L"\n");
		details::_printing(tmp, args ...);*/
		
		Print(format, args...); Print(L"\n");
	}

	template<class Tchar, class ... Args>
	void Printl(std::basic_string<Tchar> const & format, Args const & ... args) noexcept
	{
		//details::_printing(format.c_str(), args...);
		
		if (std::is_same<Tchar, char>())
		{
			details::_printing(format.c_str(), EndL_t<char>("\n"), args ...);
		}
		else
		{
			details::_printing(format.c_str(), EndL_t<wchar_t>(L"\n"), args ...);
		}
	}

	inline void Printl(wchar_t const * const value) noexcept
	{
		Print(L"%ls\n", value);
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
	// Format... - Build formated text
	//

	// Write a formated message to a buffer
	template <typename Tchar, typename ... Args>
	int FormatBuffer(Tchar * const buffer, size_t const bufferCount,
					 Tchar const * const format, Args const & ... args)
	{
		int const result = details::unsafe_format_buffer(buffer, bufferCount,
														 format, args...);
		PF_ASSERT(result != -1);
		details::ensure_valid_fmt_result(result);

		return result;
	}

	template <typename Tchar, size_t _BufferSize, typename ... Args>
	int FormatBuffer(Tchar const (&buffer)[_BufferSize],
					 Tchar const * const format, Args const & ... args)
	{
		return FormatBuffer(buffer, _BufferSize, format, args ...);
	}

	// Write a formated message to a string
	template <typename Tchar, typename ... Args>
	int FormatString(std::basic_string<Tchar> & buffer,
					 Tchar const * const format, Args const & ... args)
	{
		int size = details::unsafe_format_buffer(&buffer[0], buffer.size() + 1, 
												 format, args ...);
		
		if (size == -1)
		{
			size = details::get_required_size(format, args ...);
		}

		PF_ASSERT(size != -1);
		details::ensure_valid_fmt_result(size);

		if (static_cast<size_t>(size) > buffer.size())
		{
			buffer.resize(size);
			FormatBuffer(&buffer[0], buffer.size() + 1, format, args ...);
		}
		else if (static_cast<size_t>(size) < buffer.size())
		{
			buffer.resize(size);
		}
		
		return size;
	}

	//
	// ToString - Helper functions to simplify formating
	//

	// Converts a null-terminated wchar_t* string to a std::string
	std::string ToString(wchar_t const * value);

	// Converts a null-terminated char* string to a std::wstring
	std::wstring ToWideString(char const * value);


	std::string ToString(float const value, unsigned const precision);

	std::string ToString(double const value, unsigned const precision);

	std::wstring ToWideString(float const value, unsigned const precision);

	std::wstring ToWideString(double const value, unsigned const precision);

}


