#include "Precompiled.h"
#include "..\printf_ex\printf_ex.h"

/*
 Writing performance test

 0 = std::cout
 1 = printf
 2 = Red::Print
*/
#define TEST 2

//using namespace std;
using std::string;
using std::unordered_map;
using namespace std::chrono;
using namespace KennyKerr;
using namespace concurrency;
using namespace Red;

auto time_now() -> high_resolution_clock::time_point
{
    return high_resolution_clock::now();
}

auto time_elapsed(high_resolution_clock::time_point const & start) -> float
{
    return duration_cast<duration<float>>(time_now() - start).count();
}

class text_file
{
    char const * m_begin {};
    char const * m_end {};

    auto unmap() throw() -> void
    {
        if (m_begin)
        {
            VERIFY(UnmapViewOfFile(m_begin));
        }
    }

public:

    text_file(text_file const &) = delete;
    auto operator=(text_file const &) -> text_file & = delete;

    explicit text_file(wchar_t const * filename) throw()
    {
        auto file = invalid_handle
        {
            CreateFile(filename,
                       GENERIC_READ,
                       FILE_SHARE_READ,
                       nullptr,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       nullptr)
        };

        if (!file) return;

        auto map = null_handle
        {
            CreateFileMapping(file.get(),
                              nullptr,
                              PAGE_READONLY,
                              0, 0,
                              nullptr)
        };

        if (!map) return;

        auto size = LARGE_INTEGER {};

        if (!GetFileSizeEx(file.get(), &size)) return;

        m_begin = static_cast<char const *>(MapViewOfFile(map.get(),
                                                          FILE_MAP_READ,
                                                          0, 0,
                                                          0));

        if (!m_begin) return;

        m_end = m_begin + size.QuadPart;
    }

    ~text_file()
    {
        unmap();
    }

    text_file(text_file && other) throw() :
        m_begin(other.m_begin),
        m_end(other.m_end)
    {
        other.m_begin = nullptr;
        other.m_end = nullptr;
    }

    auto operator=(text_file && other) throw() -> text_file &
    {
        if (this != &other)
        {
            unmap();

            m_begin = other.m_begin;
            m_end = other.m_end;

            other.m_begin = nullptr;
            other.m_end = nullptr;
        }

        return *this;
    }

    explicit operator bool() const throw()
    {
        return m_begin != nullptr;
    }

    auto begin() const throw() -> char const *
    {
        return m_begin;
    }

    auto end() const throw() -> char const *
    {
        return m_end;
    }
};

auto begin(text_file const & file) throw() -> char const *
{
    return file.begin();
}

auto end(text_file const & file) throw() -> char const *
{
    return file.end();
}

using word_map = unordered_map<string, unsigned>;


// exec
auto wmain(int argc, wchar_t ** argv) -> int
{
    auto const start = time_now();
    
    auto shared = combinable<word_map> {};

    parallel_for_each(argv + 1, argv + argc, [&](wchar_t const * name)
    {
        auto f = text_file { name };

        if (!f) return;

        auto & result = shared.local();

        char const * w = nullptr;

        for (auto it = begin(f); it != end(f); ++it)
        {
            if (*it < 0 || (!isalnum(*it) && *it != '_'))
            {
                if (w)
                {
                    ++result[string(w, it)];
                }

                w = nullptr;
            }
            else if (!w)
            {
                w = it;
            }
        }

        if (w)
        {
            ++result[string(w, end(f))];
        }
    });

    word_map * result = nullptr;

    shared.combine_each([&] (word_map & local)
    {
        if (!result || result->size() < local.size())
        {
            result = &local;
        }
    });

    if (!result) return 0;

    shared.combine_each([&] (word_map const & local)
    {
        if (result != &local)
        {
            word_map & combined = *result;

            for (auto const & w : local)
            {
                combined[w.first] += w.second;
            }
        }
    });

#if TEST == 0
	string selection{ "cout: \n" };
#elif TEST == 1
	string selection{ "printf: \n" };
#elif TEST == 2
	string selection{ "PrintF_ex: \n" };
#else
#error Invalid Test selection
#endif

    for (auto const & w : *result)
    {

	#if TEST == 0
        cout << w.first << " : " << w.second << endl;
	#elif TEST == 1
        printf("%s : %d\n", w.first.c_str(), w.second);
	#elif TEST == 2
		//PrintF_ex::Printl("%s : %d", w.first, w.second);
		Print("%s : %d", Endl("\n"), w.first, w.second);
	#endif

    }

    std::cout << std::endl << selection << "Words: " << result->size()
         << " Seconds: " << time_elapsed(start) << std::endl;
}
