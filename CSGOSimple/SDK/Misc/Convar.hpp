#pragma once

#include <cstdint>

#include "../Interfaces/ICvar.hpp"
#include "UtlVector.hpp"
#include "UtlString.hpp"

#define FORCEINLINE_CVAR inline

class ConVar;
class CCommand;
class ConCommand;
class ConCommandBase;
struct CharacterSet_t;

class CCommand
{
public:
    CCommand();
    CCommand(int argC, const char** argV);
    bool Tokenize(const char* pCommand, CharacterSet_t* breakSet = NULL);
    void Reset();

    int ArgC() const;
    const char** ArgV() const;
    const char* ArgS() const;
    const char* GetCommandString() const;
    const char* operator[](int index) const;
    const char* Arg(int index) const;

    const char* FindArg(const char* name) const;
    int FindArgInt(const char* name, int defaultVal) const;

    static int MaxCommandLength();
    static CharacterSet_t* DefaultBreakSet();

private:
    enum
    {
        COMMAND_MAX_ARGC = 64,
        COMMAND_MAX_LENGTH = 512,
    };

    int m_nArgc;
    int m_nArgv0Size;
    char m_pArgSBuffer[COMMAND_MAX_LENGTH];
    char m_pArgvBuffer[COMMAND_MAX_LENGTH];
    const char* m_ppArgv[COMMAND_MAX_ARGC];
};

inline int CCommand::MaxCommandLength()
{
    return COMMAND_MAX_LENGTH - 1;
}

inline int CCommand::ArgC() const
{
    return m_nArgc;
}

inline const char** CCommand::ArgV() const
{
    return m_nArgc ? (const char**)m_ppArgv : NULL;
}

inline const char* CCommand::ArgS() const
{
    return m_nArgv0Size ? &m_pArgSBuffer[m_nArgv0Size] : "";
}

inline const char* CCommand::GetCommandString() const
{
    return m_nArgc ? m_pArgSBuffer : "";
}

inline const char* CCommand::Arg(int index) const
{
    if (index < 0 || index >= m_nArgc)
        return "";
    return m_ppArgv[index];
}

inline const char *CCommand::operator[](int index) const
{
    return Arg(index);
}

class IConCommandBaseAccessor
{
public:
    virtual bool RegisterConCommandBase(ConCommandBase* pVar) = 0;
};

typedef void(*FnCommandCallbackV1_t)(void);
typedef void(*FnCommandCallback_t)(const CCommand &command);

#define COMMAND_COMPLETION_MAXITEMS       64
#define COMMAND_COMPLETION_ITEM_LENGTH    64

typedef int(*FnCommandCompletionCallback)(const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

class ICommandCallback
{
public:
    virtual void CommandCallback(const CCommand& command) = 0;
};

class ICommandCompletionCallback
{
public:
    virtual int  CommandCompletionCallback(const char* pPartial, CUtlVector<CUtlString>& commands) = 0;
};

class ConCommandBase
{
    friend class CCvar;
    friend class ConVar;
    friend class ConCommand;
    friend void ConVar_Register(int cvarFlag, IConCommandBaseAccessor* accessor);

    friend class CDefaultCvar;

public:
    ConCommandBase(void);
    ConCommandBase(const char* name, const char* helpString = 0, int flags = 0);

    virtual ~ConCommandBase(void);
    virtual bool IsCommand(void) const;
    virtual bool IsFlagSet(int flag) const;
    virtual void AddFlags(int flags);
    virtual void RemoveFlags(int flags);
    virtual int GetFlags() const;
    virtual const char* GetName(void) const;
    virtual const char* GetHelpText(void) const;
    const ConCommandBase* GetNext(void) const;
    ConCommandBase* GetNext(void);
    virtual bool IsRegistered(void) const;
    virtual CVarDLLIdentifier_t GetDLLIdentifier() const;

    virtual void Create(const char* name, const char* helpString = 0, int flags = 0);
    virtual void Init();
    void Shutdown();
    char* CopyString(const char* from);

    ConCommandBase* m_pNext;
    bool m_bRegistered;
    const char* m_pszName;
    const char* m_pszHelpString;
    int m_nFlags;
    std::add_pointer_t<void __cdecl()> changeCallback;

protected:
    static ConCommandBase* s_pConCommandBases;
    static IConCommandBaseAccessor* s_pAccessor;

public:
    static ConCommandBase* s_pRegisteredCommands;
};

class ConCommand : public ConCommandBase
{
    friend class CCvar;

public:
    typedef ConCommandBase BaseClass;

    ConCommand(const char* name, FnCommandCallbackV1_t callback,
        const char* helpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
    ConCommand(const char* name, FnCommandCallback_t callback,
        const char* helpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0);
    ConCommand(const char* name, ICommandCallback* pCallback,
        const char* helpString = 0, int flags = 0, ICommandCompletionCallback* commandCompletionCallback = 0);

    virtual ~ConCommand(void);
    virtual bool IsCommand(void) const;
    virtual int AutoCompleteSuggest(const char* partial, CUtlVector<CUtlString>& commands);
    virtual bool CanAutoComplete(void);
    virtual void Dispatch(const CCommand& command);

    union
    {
        FnCommandCallbackV1_t m_fnCommandCallbackV1;
        FnCommandCallback_t m_fnCommandCallback;
        ICommandCallback* m_pCommandCallback;
    };

    union
    {
        FnCommandCompletionCallback m_fnCompletionCallback;
        ICommandCompletionCallback* m_pCommandCompletionCallback;
    };

    bool m_bHasCompletionCallback : 1;
    bool m_bUsingNewCommandCallback : 1;
    bool m_bUsingCommandCallbackInterface : 1;
};


class ConVar : public ConCommandBase, public IConVar
{
    friend class CCvar;
    friend class ConVarRef;
    friend class SplitScreenConVarRef;

public:
    typedef ConCommandBase BaseClass;

    ConVar(const char *name, const char *defaultValue, int flags = 0);

    ConVar(const char *name, const char *defaultValue, int flags, const char *helpString);
    ConVar(const char *name, const char *defaultValue, int flags, const char *helpString, bool bMin, float fMin, bool bMax, float fMax);
    ConVar(const char *name, const char *defaultValue, int flags, const char *helpString, FnChangeCallback_t callback);
    ConVar(const char *name, const char *pDefaultValue, int flags, const char *helpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

    virtual ~ConVar(void);
    virtual bool IsFlagSet(int flag) const;
    virtual const char* GetHelpText(void) const;
    virtual bool IsRegistered(void) const;
    virtual const char* GetName(void) const;
    virtual const char* GetBaseName(void) const;
    virtual int GetSplitScreenPlayerSlot() const;

    virtual void AddFlags(int flags);
    virtual int GetFlags() const;
    virtual bool IsCommand(void) const;

    void InstallChangeCallback(FnChangeCallback_t callback, bool bInvoke = true);

    int GetChangeCallbackCount() const { return m_pParent->m_fnChangeCallbacks.Count(); }
    FnChangeCallback_t GetChangeCallback(int slot) const { return m_pParent->m_fnChangeCallbacks[slot]; }

    virtual float GetFloat(void) const;
    virtual int GetInt(void) const;
    FORCEINLINE_CVAR Color GetColor(void) const;
    FORCEINLINE_CVAR bool GetBool() const { return !!GetInt(); }
    FORCEINLINE_CVAR char const* GetString(void) const;

    template <typename T> T Get(void) const;
    template <typename T> T Get(T*) const;

    virtual void SetValue(const char *value);
    virtual void SetValue(float value);
    virtual void SetValue(int value);
    virtual void SetValue(Color value);

    void Revert(void);
    bool GetMin(float& minVal) const;
    bool GetMax(float& maxVal) const;
    const char* GetDefault(void) const;

    struct CVValue_t
    {
        char*   m_pszString;
        int     m_StringLength;
        float   m_fValue;
        int     m_nValue;
    };

    FORCEINLINE_CVAR CVValue_t &GetRawValue()
    {
        return m_Value;
    }
    FORCEINLINE_CVAR const CVValue_t &GetRawValue() const
    {
        return m_Value;
    }

    virtual void InternalSetValue(const char *value);
    virtual void InternalSetFloatValue(float newValue);
    virtual void InternalSetIntValue(int value);
    virtual void InternalSetColorValue(Color value);
    virtual bool ClampValue(float& value);
    virtual void ChangeStringValue(const char *tempVal, float oldValue);
    virtual void Create(const char *name, const char *defaultValue, int flags = 0, const char *helpString = 0, bool bMin = false, float fMin = 0.0, bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0);

    virtual void Init();

    ConVar* m_pParent;
    const char* m_pszDefaultValue;
    CVValue_t m_Value;
    bool m_bHasMin;
    float m_fMinVal;
    bool m_bHasMax;
    float m_fMaxVal;

    CUtlVector<FnChangeCallback_t> m_fnChangeCallbacks;
};

FORCEINLINE_CVAR float ConVar::GetFloat(void) const
{
    uint32_t xored = *(uint32_t*)&m_pParent->m_Value.m_fValue ^ (uint32_t)this;
    return *(float*)&xored;
}

FORCEINLINE_CVAR int ConVar::GetInt(void) const
{
    return (int)(m_pParent->m_Value.m_nValue ^ (int)this);
}

FORCEINLINE_CVAR Color ConVar::GetColor(void) const
{
    int value = GetInt();
    unsigned char *colorElement = ((unsigned char *)&value);
    return Color(colorElement[0], colorElement[1], colorElement[2], colorElement[3]);
}

template <> FORCEINLINE_CVAR float          ConVar::Get<float>(void) const { return GetFloat(); }
template <> FORCEINLINE_CVAR int            ConVar::Get<int>(void) const { return GetInt(); }
template <> FORCEINLINE_CVAR bool           ConVar::Get<bool>(void) const { return GetBool(); }
template <> FORCEINLINE_CVAR const char*    ConVar::Get<const char *>(void) const { return GetString(); }
template <> FORCEINLINE_CVAR float          ConVar::Get<float>(float *p) const { return (*p = GetFloat()); }
template <> FORCEINLINE_CVAR int            ConVar::Get<int>(int *p) const { return (*p = GetInt()); }
template <> FORCEINLINE_CVAR bool           ConVar::Get<bool>(bool *p) const { return (*p = GetBool()); }
template <> FORCEINLINE_CVAR const char*    ConVar::Get<const char *>(char const **p) const { return (*p = GetString()); }

FORCEINLINE_CVAR const char *ConVar::GetString(void) const
{
    if(m_nFlags & FCVAR_NEVER_AS_STRING)
        return "FCVAR_NEVER_AS_STRING";
    char const *str = m_pParent->m_Value.m_pszString;
    return str ? str : "";
}

void ConVar_Register(int cvarFlag = 0, IConCommandBaseAccessor *accessor = NULL);
void ConVar_Unregister();