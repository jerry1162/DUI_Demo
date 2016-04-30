#include "Functions.h"
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
 
void GdipStartup()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void GdipShutdown()
{
	GdiplusShutdown(gdiplusToken);
}