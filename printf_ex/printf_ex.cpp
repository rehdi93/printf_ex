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

#else // not working on GCC
wstring Red::ToWideString(char const * value)
{
	PF_ASSERT(value);
	size_t len = strlen(value);
	auto result = wstring(len+1, L'_');
	//PF_VERIFY_N((size_t)-1, mbstowcs(&result[0], value, len));
	PF_VERIFY_N(-1, details::unsafe_format_buffer(&result[0], result.size(), L"%s", value));
	if (!result.back()) result.pop_back();
	return result;
}

string Red::ToString(wchar_t const * value)
{
	// make sure to pass the size in BYTES.
	PF_ASSERT(value);
	size_t len = wcslen(value) * sizeof(wchar_t);
	auto result = string(len+1, '_');
	FormatString(result, "%ls", value);
	//PF_VERIFY_N((size_t)-1, wcstombs(&result[0], value, len));
	//PF_VERIFY_N(-1, details::unsafe_format_buffer(&result[0], result.size(), "%ls", value));
	if (!result.back()) result.pop_back();
	return result;
}


#endif // _MSC_VER


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

