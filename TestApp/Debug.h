#pragma once

#ifdef _DEBUG
#include <cstdio>
#include <crtdbg.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOHELP
#include <windows.h>

#endif

#ifdef _DEBUG
#define ASSERT _ASSERTE
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define ASSERT __noop
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif

#ifdef _DEBUG
class Tracer
{
	enum Mode;

	char const * m_filename;
	unsigned m_line;
	Mode m_mode;

	static const unsigned short TruncateLen = 37;
	static const unsigned short MsgBufferSize = 512;

	template<int _FmtSize>
	int WritePrefix(wchar_t(&buffer)[_FmtSize])
	{
		return WritePrefix(buffer, _FmtSize);
	}

	int WritePrefix(wchar_t * buffer, int size);

	template<int _FmtSize>
	int WritePrefix(char(&buffer)[_FmtSize])
	{
		return WritePrefix(buffer, _FmtSize);
	}

	int WritePrefix(char * buffer, int size);

public:

	Tracer(char const * filename, unsigned const line) :
		m_filename{ filename },
		m_line{ line },
		m_mode{ Text_Only }
	{}

	Tracer(char const * filename, unsigned const line, Mode const writeMode) :
		m_filename{ filename },
		m_line{ line },
		m_mode{ writeMode }
	{}


	enum Mode
	{
		// Prefixes the message w/ the full file path + line number
		Full_File_Path,
		// Prefixes the message w/ the file path + line number truncated to around 45 chars.
		Truncated_File_Path,
		// Prefixes the message w/ the file name + line number.
		File_Name_Only,
		// Prints the message w/ no prefixes
		Text_Only
	};


	template<typename... Args>
	const void Write(wchar_t const * format, Args... args)
	{
		wchar_t buffer[MsgBufferSize];

		int count = WritePrefix(buffer);

		ASSERT(-1 != count);

		ASSERT(-1 != _snwprintf_s(buffer + count, 
								  _countof(buffer) - count, 
								  _countof(buffer) - count - 1, 
								  format, 
								  args...));

		OutputDebugStringW(buffer);
	}

	template<typename... Args, int _FmtSize>
	const void WriteLine(wchar_t const (& format)[_FmtSize], Args... args)
	{
		wchar_t tmp[_FmtSize + 2];
		wcscpy_s(tmp, format);
		wcscat_s(tmp, L"\n");

		Write(tmp, args...);
	}

	const void WriteLine(wchar_t const * message)
	{
		Write(L"%s\n", message);
	}
	
	const void Write(wchar_t const * message)
	{
		Write(L"%s", message);
	}

	const void Write(char const * message)
	{
		Write("%s", message);
	}

	const void WriteLine(char const * message)
	{
		Write("%s\n", message);
	}


	template<typename... Args>
	const void Write(char const * format, Args... args)
	{
		char buffer[MsgBufferSize];

		int count = WritePrefix(buffer);

		ASSERT(-1 != count);
		
		
		ASSERT(-1 != _snprintf_s(buffer + count,
								_countof(buffer) - count,
								_countof(buffer) - count - 1,
								format,
								args...));
		

		OutputDebugStringA(buffer);
	}

	template<typename... Args, int _FmtSize>
	const void WriteLine(char const (&format)[_FmtSize], Args... args)
	{
		char tmp[_FmtSize + 2];
		strcpy_s(tmp, format);
		strcat_s(tmp, "\n");

		Write(tmp, args...);
	}
};

#define TRACE (Tracer(__FILE__,__LINE__))
#define TRACE_F (Tracer(__FILE__,__LINE__, Tracer::Full_File_Path))
#define TRACE_T (Tracer(__FILE__,__LINE__, Tracer::Truncated_File_Path))
#define TRACE_FN (Tracer(__FILE__,__LINE__, Tracer::File_Name_Only))
#else
#define TRACE __noop
#define TRACE_F __noop
#define TRACE_T __noop
#define TRACE_FN __noop
#endif


#ifdef _DEBUG

#include <string>
#include <codecvt>

int Tracer::WritePrefix(char * buffer, int const size)
{
	int count = -1;
	Mode mn = m_mode;

	auto const mfSize = strlen(m_filename) + 1;
	char * fmt = "%s (%d): ";

	restart:
	switch (mn)
	{
		case Tracer::Full_File_Path:
			count = sprintf_s(buffer, size, fmt, m_filename, m_line);
			break;
		case Tracer::Truncated_File_Path:
		{
			if (mfSize <= TruncateLen)
			{
				mn = Full_File_Path;
				goto restart;
			}

			std::string tmp{ "..." };
			const char * trFP = m_filename + mfSize - TruncateLen;

			tmp += trFP;

			count = sprintf_s(buffer, size, fmt, tmp.c_str(), m_line);
			break;
		}
		case Tracer::File_Name_Only:
		{
			std::string pfx(m_filename);

			auto filePos = pfx.find_last_of('\\');

			if (filePos != std::string::npos)
			{
				pfx = pfx.substr(filePos);
				pfx.insert(0, "...");
			}

			count = sprintf_s(buffer, size, fmt, pfx.c_str(), m_line);
			break;
		}
		case Tracer::Text_Only:
			return 0;
	}

	return count;
}


int Tracer::WritePrefix(wchar_t * buffer, int const size)
{
	int count = -1;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	Mode mn = m_mode;

	auto const mfSize = strlen(m_filename) + 1;
	wchar_t * fmt = L"%ls (%d): ";

	restart:
	switch (mn)
	{
		case Tracer::Full_File_Path:
			// %S permite que char * sejam formatadas em WideChars
			count = swprintf_s(buffer, size, L"%S (%d): ", m_filename, m_line);
			break;
		case Tracer::Truncated_File_Path:
		{
			if (mfSize <= TruncateLen)
			{
				mn = Full_File_Path;
				goto restart;
			}

			std::wstring wPfxFn{ L"..." };
			const char * trFP = m_filename + mfSize - TruncateLen;
			auto converted = converter.from_bytes(trFP);

			wPfxFn += converted;

			count = swprintf_s(buffer, size, fmt, wPfxFn.c_str(), m_line);
			break;
		}
		case Tracer::File_Name_Only:
		{
			std::wstring pfx = converter.from_bytes(m_filename);
			auto filePos = pfx.find_last_of('\\');

			if (filePos != std::string::npos)
			{
				pfx = pfx.substr(filePos);
				pfx.insert(0, L"...");
			}

			count = swprintf_s(buffer, size, fmt, pfx.c_str(), m_line);
			break;
		}
		case Tracer::Text_Only:
			return 0;
	}

	return count;
}

#endif // _DEBUG
