#pragma once
// Printf_ex debug header

#ifdef _DEBUG
	#ifdef _MSC_VER
		#include <crtdbg.h>
		#define PF_ASSERT _ASSERTE
	#else
		#include <cassert>
		#define PF_ASSERT assert
	#endif // _MSC_VER

	#define PF_VERIFY PF_ASSERT
	#define PF_VERIFY_(result, expression) PF_ASSERT(result == expression)
	#define PF_VERIFY_N(badresult, expression) PF_ASSERT(badresult != expression)

#else

#ifdef _MSC_VER
	#define PF_ASSERT __noop
#else
	#define PF_ASSERT(ignore) ((void)0)
#endif // _MSC_VER

	#define PF_VERIFY(expression) (expression)
	#define PF_VERIFY_(result, expression) (expression)
	#define PF_VERIFY_N(badresult, expression) (expression)

#endif // _DEBUG