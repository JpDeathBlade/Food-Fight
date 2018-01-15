#include "CStringTable.h"
#include <fstream>

CStringTable* CStringTable::m_Instance = NULL;

CStringTable::CStringTable()
{

}

CStringTable::~CStringTable()
{
	m_Instance = NULL;
}

CStringTable* CStringTable::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CStringTable;
	return m_Instance;
}

void CStringTable::DeleteInstance()
{
	delete m_Instance;
}

void CStringTable::LoadDictionary(const char* FileName)
{
	std::ifstream File(FileName);
	if(File.is_open())
	{
		while(!File.eof())
		{
			std::string Key;
			std::getline(File, Key);
			std::string Value;
			std::getline(File, Value);
			m_Strings[Key] = Value;
		}
	}
	else
		m_Strings.clear();
	File.close();
}

std::string CStringTable::FindString(std::string Key)
{
	std::map<std::string, std::string>::iterator Iterator = m_Strings.find(Key);
	if(Iterator != m_Strings.end())
		return (Iterator->second);
	else
		return Key;
}