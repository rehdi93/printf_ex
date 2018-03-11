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
//
//template<class Tchar, class Uchar, class UtoTFunc>
//basic_string<Tchar> BufferToStr(Uchar const * value, size_t const len, UtoTFunc convert)
//{
//	PF_ASSERT(value);
//	size_t n{};
//	basic_string<Tchar> result(len, 0);
//	convert(&n, &result[0], len, value, len - 1);
//	result.resize(n);
//	result.pop_back(); // remove extra null terminator
//	return restr
//}


wstring Red::ToWideString(char const * value)
{
	PF_ASSERT(value);
	size_t n{};
	size_t size = strlen(value) + 1;
	wstring tmp(size, L'f');
	mbstowcs_s(&n, &tmp[0], size, value, size - 1);
	tmp.resize(n);
	tmp.pop_back(); // remove extrainious null terminator
	return tmp;
}

string Red::ToString(wchar_t const * value)
{
	PF_ASSERT(value);
	size_t n{};
	// duplicate the size to compensate for multibyte chars.
	size_t size = (wcslen(value) + 1) * 2;
	string tmp(size, 'f');
	wcstombs_s(&n, &tmp[0], size, value, size - 1);
	tmp.resize(n);
	tmp.pop_back(); // remove extrainious null terminator
	return tmp;
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

