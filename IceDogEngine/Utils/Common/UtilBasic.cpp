
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

	std::vector<char> ReadAllBytes(char const* filename)
	{
		std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
		std::ifstream::pos_type pos = ifs.tellg();

		std::vector<char> result(pos);

		ifs.seekg(0, std::ios::beg);
		ifs.read(&result[0], pos);

		return result;
	}

}