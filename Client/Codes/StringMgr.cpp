#include "stdafx.h"
#include "StringMgr.h"


StringMgr::StringMgr()
{

}


StringMgr::~StringMgr()
{
}

bool StringMgr::FindString(char* pAnimBuffer, const char* pAnimTag, const char* pFindTag)
{
	size_t length = strlen(pAnimTag);
	strcpy_s(pAnimBuffer, length + 1, pAnimTag);

	for (size_t i = 0; i < length; i++)
	{
		if (strcmp(pAnimTag + i, pFindTag) == 0)
		{
			return true;
		}
	}

	return false;

}
