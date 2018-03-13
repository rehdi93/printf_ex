#include "../printf_ex/printf_ex.h"
#include <array>

#if defined _MSC_VER
	#define FMT_NARROW_STR_IN_WIDE L"%S"
	#define TPS_REPORTS_W L"Tps Reports:\n%d) %s\n%d) %s %.2f"
#elif defined __GNUG__
	#define FMT_NARROW_STR_IN_WIDE L"%s"
	#define TPS_REPORTS_W L"Tps Reports:\n%d) %ls\n%d) %ls %.2f"
#endif // _MSC_VER

#define RAW_BUFFER 256

auto lorem_ipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus velit quam, consequat a hendrerit non, tempor in elit. Quisque eu risus at neque eleifend posuere sed id odio.";

using namespace Red;
using std::string;
using std::wstring;
using std::array;


void TestRun()
{
	Printl("Starting PrintF_ex tests...");

	string fmt = "%d %d %d - %s";
	Printl(fmt, 3, 2, 1, "Go!");
	
	string const SEP(10, '=');

	Printl(SEP + " MESSEGES " + SEP);

	Printl("This is a messege. %d - %d", 123, 456);
	Printl("Here's a c string: %s", "Hello world");

	wstring printStrings{ L"Print messages stored in std strings!" };
	Printl(printStrings);

	Print("Start of a line... "); Print("-- %d --", 4321); Printl(" ...end of a line.");

	Printl(L"This is a wide message");
	Print("\n");

	Printl(SEP + " ToString " + SEP);
	
	Print("\nPrinting numerics w/ ToString", Endl('\n',2));

	string expected = "123.46";
	string actual = ToString(123.457, 2);

	Print("Expected: %s, Actual %s - ", expected, actual);
	PF_ASSERT(expected == actual);
	Printl("Ok!");


	auto number = rand() / 2.3;
	Printl("A number is %s", ToString(number, 4));

	Print("\n");
	
	Printl("wchar_t * to string:");

#ifndef __GNUG__
	auto wcBefore = L"The wide cháràcter fõx jumpêd over the lazy dog.";
	auto strAfter = ToString(wcBefore);

	Printl("- Before:\t%ls", wcBefore);
	Printl("- After:\t%s", strAfter);

	Print("\n");
#else
	Printl("Not working in GCC...");
#endif // !__GNUG__

	Printl("char * to wstring:");

#ifndef __GNUG__
	const char * cBefore = "The wide cháràcter fõx jumpêd over the lazy dog.";
	auto wstrAfter = ToWideString(cBefore);

	Printl("- Before:\t%s", cBefore);
	Printl(L"- After:\t%s", wstrAfter);
#else
	Printl("Not working in GCC...");
#endif // !__GNUG__

	Print("\n");
	Printl(SEP + " Padding " + SEP);
	Print("\n");

	// Some names
	array<string, 5> saleReps = 
	{
		"Fulano", "Ciclano",
		"Beltrano", "José",
		"Maria"
	};

	Printl("SALES REPORT\n");
	for (size_t i = 0; i < saleReps.size(); i++)
	{
		int salesNum = rand();

		Print("%-10s", saleReps[i]);
		Printl("%d items", salesNum);
	}

	Print("\n\n");

	Printl(SEP + " FormatString and FormatBuffer " + SEP);
	Printl("Use FormatString to formating a message to a std::string!");
	Print("\n\n");

	Print("Utf8:", endline(2));

	string customFmt;
	FormatString(customFmt, "Tps Reports:\n%d) %s\n%d) %s %.2f",
				 1, "Needs a new tonner!",
				 2, "Budget", 654.87452f);

	Print(customFmt, endline(3));
	Print("Utf16:", endline(2));

	wstring customFmtWide;
	FormatString(customFmtWide, TPS_REPORTS_W,
				 1, L"Needs a new tonner!",
				 2, L"Budget", 654.87452f);

	customFmtWide += L"\n\n\n";
	Print(customFmtWide);

	Print("Or, if you need to format a c-style string, use FormatBuffer", endline(2));

	wchar_t customfmtraw[RAW_BUFFER];
	FormatBuffer(customfmtraw, RAW_BUFFER, FMT_NARROW_STR_IN_WIDE, lorem_ipsum);

	Printl("%ls", customfmtraw);

	Print("\n\n");


	Printl(SEP + "Define a custom endline!" + SEP);

	unsigned nlCount = 5;
	Print("Printing %d new lines after this message", endline(nlCount), nlCount);
}



int main()
{
	
	TestRun();
	
	return 0;
}