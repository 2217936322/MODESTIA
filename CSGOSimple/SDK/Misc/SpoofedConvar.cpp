#include "SpoofedConvar.hpp"

#include "../SDK.hpp"

SpoofedConvar::SpoofedConvar(const char* szCVar) {
	m_pOriginalCVar = g_CVar->FindVar(szCVar);
	Spoof();
}
SpoofedConvar::SpoofedConvar(ConVar* pCVar) {
	m_pOriginalCVar = pCVar;
	Spoof();
}
SpoofedConvar::~SpoofedConvar() {
	if (IsSpoofed()) {
		DWORD dwOld;

		//Restore flags
		SetFlags(m_iOriginalFlags);
		//Restore value
		SetString(m_szOriginalValue);
		//Restore the name
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, PAGE_READWRITE, &dwOld);
		strcpy((char*)m_pOriginalCVar->m_pszName, m_szOriginalName);
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, dwOld, &dwOld);

		//Unregister dummy cvar
		g_CVar->UnregisterConCommand(m_pDummyCVar);
		free(m_pDummyCVar);
		m_pDummyCVar = nullptr;
	}
}
bool SpoofedConvar::IsSpoofed() {
	return m_pDummyCVar != nullptr;
}
void SpoofedConvar::Spoof() {
	if (!IsSpoofed() && m_pOriginalCVar) {
		//Save old name value and flags so we can restore the cvar lates if needed
		m_iOriginalFlags = m_pOriginalCVar->m_nFlags;
		strcpy(m_szOriginalName, m_pOriginalCVar->m_pszName);
		strcpy(m_szOriginalValue, m_pOriginalCVar->GetString());

		sprintf_s(m_szDummyName, 128, "dummy_%s", m_szOriginalName);

		//Create the dummy cvar
		m_pDummyCVar = (ConVar*)malloc(sizeof(ConVar));
		if (!m_pDummyCVar) return;
		memcpy(m_pDummyCVar, m_pOriginalCVar, sizeof(ConVar));

		m_pDummyCVar->m_pNext = nullptr;
		//Register it
		g_CVar->RegisterConCommand(m_pDummyCVar);

		//Fix "write access violation" bullshit
		DWORD dwOld;
		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, PAGE_READWRITE, &dwOld);

		//Rename the cvar
		strcpy((char*)m_pOriginalCVar->m_pszName, m_szDummyName);

		VirtualProtect((LPVOID)m_pOriginalCVar->m_pszName, 128, dwOld, &dwOld);

		SetFlags(FCVAR_NONE);
	}
}
void SpoofedConvar::SetFlags(int flags) {
	if (IsSpoofed()) {
		m_pOriginalCVar->m_nFlags = flags;
	}
}
int SpoofedConvar::GetFlags() {
	return m_pOriginalCVar->GetFlags();
}
void SpoofedConvar::SetInt(int iValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(iValue);
	}
}
void SpoofedConvar::SetBool(bool bValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(bValue);
	}
}
void SpoofedConvar::SetFloat(float flValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(flValue);
	}
}
void SpoofedConvar::SetString(const char* szValue) {
	if (IsSpoofed()) {
		m_pOriginalCVar->SetValue(szValue);
	}
}
int SpoofedConvar::GetInt() {
	return m_pOriginalCVar->GetInt();
}
float SpoofedConvar::GetFloat() {
	return m_pOriginalCVar->GetFloat();
}
const char* SpoofedConvar::GetString() {
	return m_pOriginalCVar->GetString();
}