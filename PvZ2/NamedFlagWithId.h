#pragma once

class NamedFlagWithId
{
public:
	SexyString m_namedFlag;
	int m_id;

	NamedFlagWithId() : m_namedFlag(""), m_id(0) {};
	NamedFlagWithId(SexyString namedFlag, int id) : m_namedFlag(namedFlag), m_id(id) {};
};