#include <stdarg.h>
#include <stdio.h>

#define MY_VAR_FUNC(fmt,...) \
do { \
    if (0) \
    { \
     printf(fmt,__VA_ARGS__); \
    } \
    MyVarFunc(fmt,__VA_ARGS__); \
} while(false);

void MyVarFunc(const char* szFormat, ...)
{
	va_list lpStart;
	va_start(lpStart, szFormat);

	vprintf(szFormat, lpStart);

	va_end(lpStart);
}

int main()
{
	int a = 5, b = 4;
	MY_VAR_FUNC("%d + %d = %d\n", a, b, a + b);
	return 0;
}
