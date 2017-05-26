
#include "UtilBasic.h"

namespace IceDogUtils
{
	wchar_t* char2wchar(char* CStr)
	{
		size_t len = strlen(CStr) + 1;
		size_t converted = 0;
		wchar_t* WStr;
		WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
		mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
		return WStr;
	};
}