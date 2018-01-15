#ifndef _CStringTable_H_
#define _CStringTable_H_
#include <string>
#include <map>

class CStringTable
{
	static CStringTable* m_Instance;
	std::map<std::string, std::string> m_Strings;

	CStringTable();
	CStringTable(const CStringTable& Source);
	CStringTable& operator=(const CStringTable& Source);
	~CStringTable();
public:
	static CStringTable* GetInstance();
	static void DeleteInstance();

	void LoadDictionary(const char* FileName);
	std::string FindString(std::string Key);
};
#endif