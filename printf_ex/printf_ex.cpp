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



#ifdef _MSC_VER
template<class Tchar, class Uchar>
using ConversionHandler = errno_t (*)(size_t *, Tchar*, size_t const, Uchar const * const, size_t);

template<class Tchar, class Uchar>
basic_string<Tchar> BufferToStr(Uchar const * value, size_t const len, ConversionHandler<Tchar, Uchar> convert)
{
	PF_ASSERT(value);
	size_t n{};
	basic_string<Tchar> result(len, 0);
	PF_VERIFY_(0, convert(&n, &result[0], len, value, len - 1));
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
	// make sure to pass the size in BYTES.
	return BufferToStr<char>(value, (wcslen(value) + 1) * sizeof(wchar_t), wcstombs_s);
}
#endif // _MSC_VER

#ifdef __GNUG__

template<class I, class E, class S>
struct codecvt : public std::codecvt<I, E, S>
{
	~codecvt() {}
};

using StrConverter = std::wstring_convert<codecvt<wchar_t, char, std::mbstate_t>>;


wstring Red::ToWideString(char const * value)
{
	PF_ASSERT(value);

	StrConverter conv16;
	auto result = conv16.from_bytes(value);
	
	return result;
}

string Red::ToString(wchar_t const * value)
{
	PF_ASSERT(value);

	StrConverter conv16;
	auto result = conv16.to_bytes(value);

	return result;
}
#endif // __GNUG__



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

