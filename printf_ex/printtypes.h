#pragma once
// Printf_ex types

#include <string>

namespace PrintF_ex
{
	template<class Tchar>
	struct EndL_t
	{
		std::basic_string<Tchar> value;

		EndL_t(Tchar const * _c)
			: value{ _c }
		{
		}

		EndL_t(Tchar const _c, unsigned count)
			: value(count, _c)
		{
		}

	};

	using Endl = EndL_t<char>;
	using wEndl = EndL_t<wchar_t>;

}

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
