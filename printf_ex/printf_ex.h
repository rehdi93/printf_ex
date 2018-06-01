#pragma once
// Printf_ex main header
// made by: Pedro Oliva Rodriges
// ref: https://msdn.microsoft.com/magazine/dn913181


#include "printf_ex_details.h"

namespace Red
{
	//
	// Print - Wrappers around printf + some shortcuts
	//

	template <typename Tchar, class ... Args>
	inline void Print(Tchar const * format, Args const & ... args) noexcept
	{
		details::internal_print(format, args ...);
	}

	inline void Print(char const * const value) noexcept
	{
		Print("%s", value);
	}

	inline void Print(wchar_t const * const value) noexcept
	{
		Print(L"%ls", value);
	}


	template <typename T>
	inline void Print(std::basic_string<T> const & value) noexcept
	{
		Print(value.c_str());
	}

	template<class Tchar, class ... Args>
	inline void Print(std::basic_string<Tchar> const & format, Args const & ... args) noexcept
	{
		Print(format.c_str(), args ...);
	}

	//
	// Printl - Shorthand for writing a message and adding a new line at the end
	//

	template <typename ... Args>
	inline void Printl(char const * format, Args const & ... args) noexcept
	{
		Print(format, args...); Print("\n");
	}

	template <typename ... Args>
	inline void Printl(wchar_t const * format, Args const & ... args) noexcept
	{
		Print(format, args...); Print(L"\n");
	}

	template<class Tchar, class ... Args>
	inline void Printl(std::basic_string<Tchar> const & format, Args const & ... args) noexcept
	{
		Printl(format.c_str(), args...);
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
	inline void Printl(std::basic_string<T> const & value) noexcept
	{
		Printl(value.c_str());
	}

	//
	// FormatString / FormatBuffer - Build formated text
	//

	// Write a formated message to a buffer
	template <typename Tchar, typename ... Args>
	inline void FormatBuffer(Tchar * const buffer, size_t const bufferCount,
					  Tchar const * const format, Args const & ... args)
	{
		PF_ASSERT(buffer && format); // buffer and format must not be null

		int const result = details::internal_format_buffer(buffer, bufferCount,
														   format, args...);

		PF_ASSERT(result != -1 && (size_t)result < bufferCount); // formating was not sucessfull
	}

	// Write a formated message to a string
	template <typename Tchar, typename ... Args>
	inline void FormatString(std::basic_string<Tchar> & buffer,
					  Tchar const * const format, Args const & ... args)
	{
		int size = details::internal_format_buffer(&buffer[0], buffer.size() + 1, 
												   format, args ...);
		
		if (size == -1)
		{
			size = details::get_required_size(format, args ...);
		}

		PF_ASSERT(size != -1); // get_required_size should never fail
		auto sizeU = static_cast<size_t>(size);

		if (sizeU > buffer.size())
		{
			buffer.resize(sizeU);
			FormatBuffer(&buffer[0], buffer.size() + 1, format, args ...);
		}
		else if (sizeU < buffer.size())
		{
			buffer.resize(sizeU);
		}
	}

	template <typename Tchar, typename ... Args>
	inline auto FormatString(Tchar const * const format, Args const & ... args)
	{
		std::basic_string<Tchar> formated_str;
		FormatString(formated_str, format, args ...);
		return formated_str;
	}

#ifndef PFEX_HEADER_ONLY

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

#endif // !PFEX_HEADER_ONLY

}


