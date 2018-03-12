#pragma once
// Printf_ex types

#include <string>

namespace Red
{
	template<class Tchar>
	struct EndL_t
	{
		std::basic_string<Tchar> value;

		EndL_t(Tchar const * _c)
			: value{ _c } {}

		EndL_t(Tchar const _c, unsigned count)
			: value(count, _c) {}


		auto operator() () const
		{
			return value;
		}
	};

	using Endl = EndL_t<char>;
	using wEndl = EndL_t<wchar_t>;

	inline Endl endline(unsigned const newLines)
	{
		return Endl('\n', newLines);
	}

	inline wEndl wendline(unsigned const newLines)
	{
		return wEndl(L'\n', newLines);
	}
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
