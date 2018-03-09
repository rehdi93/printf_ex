#pragma once
// Printf_ex debug header

#ifdef _DEBUG
#include <crtdbg.h>

#define PF_ASSERT _ASSERTE
#define PF_VERIFY PF_ASSERT
#define PF_VERIFY_(result, expression) PF_ASSERT(result == expression)
#else
#define PF_ASSERT __noop
#define PF_VERIFY(expression) (expression)
#define PF_VERIFY_(result, expression) (expression)
#endif // _DEBUG