#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

#ifdef _DEBUG
#define ASSERT _ASSERTE
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define ASSERT __noop
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif // _DEBUG