#ifndef STRING_MGR_H__
#define STRING_MGR_H__

#include "Base.h"
#include "Include.h"

class StringMgr
{
public:
	StringMgr();
	~StringMgr();

public:
	static bool FindString(char* pAnimBuffer, const char* pAnimTag, const char* pFindTag);

private:
	char* m_pAnimBuffer = nullptr;
};

#endif

