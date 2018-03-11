#include "printf_ex.h"

using std::string;
using std::wstring;
using std::basic_string;
using std::is_same;


template<class Tnum>
auto NumToStr(Tnum const value, unsigned const precision)
{
	string result; 
	Red::FormatString(result, "%.*f", precision, value);
	return result;
}	

template<class Tnum>
auto NumToWideStr(Tnum const value, unsigned const precision)
{
	wstring result;
	Red::FormatString(result, L"%.*f", precision, value);
	return result;
}

template<class Tchar, class Uchar>
using ConversionHandler = errno_t (*)(size_t *, Tchar*, size_t const, Uchar const * const, size_t);

template<class Tchar, class Uchar>
basic_string<Tchar> BufferToStr(Uchar const * value, size_t const len, ConversionHandler<Tchar, Uchar> convert)
{
	PF_ASSERT(value);
	size_t n{};
	basic_string<Tchar> result(len, 0);
	convert(&n, &result[0], len, value, len - 1);
	result.resize(n);
	result.pop_back(); // remove extra null terminator
	return result;
}


wstring Red::ToWideString(char const * value)
{
	return BufferToStr<wchar_t>(value, strlen(value) + 1, mbstowcs_s);
}

string Red::ToString(wchar_t const * value)
{
	// duplicate the size to compensate for multibyte chars.
	return BufferToStr<char>(value, (wcslen(value) + 1) * 2, wcstombs_s);
}


string Red::ToString(float const value, unsigned const precision)
{
	return NumToStr(value, precision);
}

string Red::ToString(double const value, unsigned const precision)
{
	return NumToStr(value, precision);
}

wstring Red::ToWideString(float const value, unsigned const precision)
{
	return NumToWideStr(value, precision);
}

wstring Red::ToWideString(double const value, unsigned const precision)
{
	return NumToWideStr(value, precision);
}

