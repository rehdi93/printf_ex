#pragma once
// Part of: Printf_ex
// Copyright 2018-present Pedro Oliva Rodriges
// This code is released under the MIT licence

// based on https://msdn.microsoft.com/magazine/dn913181

#include <cstdio>
#include <string>
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

			_printing(tmp.c_str(), args ...);
		}


		template<typename ... Args>
		void _printing(wchar_t const * format, EndL_t<wchar_t> endl, Args const & ... args)
		{
			std::wstring tmp(format);
			tmp += endl.value;

			_printing(tmp.c_str(), args ...);
		}

		template<typename ... Args>
		void _printing(char const * format, Args const & ... args)
		{
			printf_s(format, Printable(args) ...);
		}

		template<typename ... Args>
		void _printing(wchar_t const * format, Args const & ... args)
		{
			wprintf_s(format, Printable(args) ...);
		}

	}

	//
	// Print - Wrappers around printf + some shortcuts
	//

	//template <typename ... Args>
	//void Print(char const * const format,
	//			Args const & ... args) noexcept
	//{
	//	details::_printing(format, args ...);
	//}

	//template <typename ... Args>
	//void Print(wchar_t const * const format,
	//			Args const & ... args) noexcept
	//{
	//	details::_printing(format, args ...);
	//}

	template <typename Tchar, class ... Args>
	void Print(Tchar const * format, Args const & ... args)
	{
		details::_printing(format, args ...);
	}

	// Print a formated message w/ a custom end line
	//template<class ... Args>
	//void Print(char const * format, EndL_t<char> endl, Args const & ... args)
	//{
	//	details::_printing(format, endl, args...);
	//}

	// Print a formated message w/ a custom end line
	//template<class ... Args>
	//void Print(wchar_t const * format, EndL_t<wchar_t> endl, Args const & ... args)
	//{
	//	details::_printing(format, endl, args...);
	//}

	template<typename Tchar, class ... Args>
	void Print(Tchar const * format, EndL_t<Tchar> endl, Args const & ... args)
	{
		details::_printing(format, endl, args ...);
	}

	inline void Print(char const * const value) noexcept
	{
		Print("%s", value);
	}

	// Print a message w/ a custom end line
	inline void Print(char const * value, EndL_t<char> const endl)
	{
		details::_printing("%s", endl, value);
	}

	inline void Print(wchar_t const * const value) noexcept
	{
		Print(L"%s", value);
	}

	// Print a message w/ a custom end line
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
	void Print(std::basic_string<Tchar> const & format,
			   Args const & ... args)
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
	void Printl(char const (&format)[_FmtSize],
				Args const & ... args) noexcept
	{
		char tmp[_FmtSize + 1];
		strcpy_s(tmp, format);
		strcat_s(tmp, "\n");

		details::_printing(tmp, args ...);
	}

	template <typename ... Args, size_t _FmtSize>
	void Printl(wchar_t const (&format)[_FmtSize],
				Args const & ... args) noexcept
	{
		wchar_t tmp[_FmtSize + 1];
		wcscpy_s(tmp, format);
		wcscat_s(tmp, L"\n");

		details::_printing(tmp, args ...);
	}

	template<class Tchar, class ... Args>
	void Printl(std::basic_string<Tchar> const & format,
				Args const & ... args)
	{
		Printl(format.c_str(), args ...);
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
}


