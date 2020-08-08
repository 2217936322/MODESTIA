#include "Convar.hpp"

#include "../SDK.hpp"

#include "CharacterSet.hpp"
#include "UtlBuffer.hpp"

#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) 
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )

ConCommandBase* ConCommandBase::s_pConCommandBases = NULL;
ConCommandBase* ConCommandBase::s_pRegisteredCommands = NULL;
IConCommandBaseAccessor* ConCommandBase::s_pAccessor = NULL;
static int s_nDLLIdentifier = -1;
static int s_nCVarFlag = 0;
static bool s_bRegistered = false;

class CDefaultAccessor : public IConCommandBaseAccessor
{
public:
    virtual bool RegisterConCommandBase(ConCommandBase* var)
    {
        g_CVar->RegisterConCommand(var);
        return true;
    }
};

static CDefaultAccessor s_DefaultAccessor;

void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor* accessor)
{
    if (!g_CVar || s_bRegistered)
        return;

    assert(s_nDLLIdentifier < 0);
    s_bRegistered = true;
    s_nCVarFlag = nCVarFlag;
    s_nDLLIdentifier = g_CVar->AllocateDLLIdentifier();

    ConCommandBase* pCur, *pNext;

    ConCommandBase::s_pAccessor = accessor ? accessor : &s_DefaultAccessor;
    pCur = ConCommandBase::s_pConCommandBases;

    while (pCur) 
    {
        pNext = pCur->m_pNext;
        pCur->AddFlags(s_nCVarFlag);
        pCur->Init();

        ConCommandBase::s_pRegisteredCommands = pCur;

        pCur = pNext;
    }

    ConCommandBase::s_pConCommandBases = NULL;
}

void ConVar_Unregister()
{
    if (!g_CVar || !s_bRegistered)
        return;

    assert(s_nDLLIdentifier >= 0);
    g_CVar->UnregisterConCommands(s_nDLLIdentifier);
    s_nDLLIdentifier = -1;
    s_bRegistered = false;
}

ConCommandBase::ConCommandBase(void)
{
    m_bRegistered = false;
    m_pszName = NULL;
    m_pszHelpString = NULL;

    m_nFlags = 0;
    m_pNext = NULL;
}

ConCommandBase::ConCommandBase(const char* name, const char* helpString, int flags)
{
    Create(name, helpString, flags);
}

ConCommandBase::~ConCommandBase(void)
{
}

bool ConCommandBase::IsCommand(void) const
{
    return true;
}

CVarDLLIdentifier_t ConCommandBase::GetDLLIdentifier() const
{
    return s_nDLLIdentifier;
}

void ConCommandBase::Create(const char* name, const char* helpString, int flags)
{
    static const char* emptyString = "";

    m_bRegistered = false;

    m_pszName = name;
    m_pszHelpString = helpString ? helpString : emptyString;

    m_nFlags = flags;

    if (!(m_nFlags & FCVAR_UNREGISTERED)) 
    {
        m_pNext = s_pConCommandBases;
        s_pConCommandBases = this;
    }
    else 
    {
        m_pNext = NULL;
    }
}

void ConCommandBase::Init()
{
    if (s_pAccessor) 
    {
        s_pAccessor->RegisterConCommandBase(this);
    }
}

void ConCommandBase::Shutdown()
{
    if (g_CVar) 
    {
        g_CVar->UnregisterConCommand(this);
    }
}

const char* ConCommandBase::GetName(void) const
{
    return m_pszName;
}

bool ConCommandBase::IsFlagSet(int flag) const
{
    return (flag & m_nFlags) ? true : false;
}

void ConCommandBase::AddFlags(int flags)
{
    m_nFlags |= flags;
}

void ConCommandBase::RemoveFlags(int flags)
{
    m_nFlags &= ~flags;
}

int ConCommandBase::GetFlags(void) const
{
    return m_nFlags;
}

const ConCommandBase* ConCommandBase::GetNext(void) const
{
    return m_pNext;
}

ConCommandBase* ConCommandBase::GetNext(void)
{
    return m_pNext;
}

char* ConCommandBase::CopyString(const char* from)
{
    int		len;
    char* to;

    len = strlen(from);
    if (len <= 0) 
    {
        to = new char[1];
        to[0] = 0;
    }
    else 
    {
        to = new char[len + 1];
        strncpy_s(to, len + 1, from, len + 1);
    }
    return to;
}

const char* ConCommandBase::GetHelpText(void) const
{
    return m_pszHelpString;
}

bool ConCommandBase::IsRegistered(void) const
{
    return m_bRegistered;
}

static CharacterSet_t s_BreakSet;
static bool s_bBuiltBreakSet = false;

CCommand::CCommand()
{
    if (!s_bBuiltBreakSet) 
    {
        s_bBuiltBreakSet = true;
        CharacterSetBuild(&s_BreakSet, "{}()':");
    }

    Reset();
}

CCommand::CCommand(int nArgC, const char** ppArgV)
{
    assert(nArgC > 0);

    if (!s_bBuiltBreakSet)
    {
        s_bBuiltBreakSet = true;
        CharacterSetBuild(&s_BreakSet, "{}()':");
    }

    Reset();

    char* pBuf = m_pArgvBuffer;
    char* pSBuf = m_pArgSBuffer;
    m_nArgc = nArgC;

    for (int i = 0; i < nArgC; ++i) 
    {
        m_ppArgv[i] = pBuf;
        int nLen = strlen(ppArgV[i]);
        memcpy(pBuf, ppArgV[i], nLen + 1);
        if (i == 0) 
        {
            m_nArgv0Size = nLen;
        }
        pBuf += nLen + 1;

        bool bContainsSpace = strchr(ppArgV[i], ' ') != NULL;
        if (bContainsSpace) 
        {
            *pSBuf++ = '\"';
        }
        memcpy(pSBuf, ppArgV[i], nLen);
        pSBuf += nLen;
        if (bContainsSpace) 
        {
            *pSBuf++ = '\"';
        }

        if (i != nArgC - 1) 
        {
            *pSBuf++ = ' ';
        }
    }
}

void CCommand::Reset()
{
    m_nArgc = 0;
    m_nArgv0Size = 0;
    m_pArgSBuffer[0] = 0;
}

CharacterSet_t* CCommand::DefaultBreakSet()
{
    return &s_BreakSet;
}

bool CCommand::Tokenize(const char* command, CharacterSet_t* breakSet)
{
    Reset();
    if (!command)
        return false;

    if (!breakSet) 
    {
        breakSet = &s_BreakSet;
    }

    int nLen = strlen(command);
    if (nLen >= COMMAND_MAX_LENGTH - 1) 
    {
        return false;
    }

    memcpy(m_pArgSBuffer, command, nLen + 1);

    CUtlBuffer bufParse(m_pArgSBuffer, nLen, CUtlBuffer::TEXT_BUFFER | CUtlBuffer::READ_ONLY);
    int nArgvBufferSize = 0;

    while (bufParse.IsValid() && (m_nArgc < COMMAND_MAX_ARGC)) 
    {
        char* pArgvBuf = &m_pArgvBuffer[nArgvBufferSize];
        int nMaxLen = COMMAND_MAX_LENGTH - nArgvBufferSize;
        int nStartGet = bufParse.TellGet();
        int	nSize = bufParse.ParseToken(breakSet, pArgvBuf, nMaxLen);
        if (nSize < 0)
            break;

        if (nMaxLen == nSize) 
        {
            Reset();
            return false;
        }

        if (m_nArgc == 1) 
        {
            m_nArgv0Size = bufParse.TellGet();
            bool bFoundEndQuote = m_pArgSBuffer[m_nArgv0Size - 1] == '\"';
            if (bFoundEndQuote) 
            {
                --m_nArgv0Size;
            }

            m_nArgv0Size -= nSize;
            assert(m_nArgv0Size != 0);

            bool bFoundStartQuote = (m_nArgv0Size > nStartGet) && (m_pArgSBuffer[m_nArgv0Size - 1] == '\"');
            assert(bFoundEndQuote == bFoundStartQuote);
            if (bFoundStartQuote) 
            {
                --m_nArgv0Size;
            }
        }

        m_ppArgv[m_nArgc++] = pArgvBuf;
        if (m_nArgc >= COMMAND_MAX_ARGC) 
        {
        }

        nArgvBufferSize += nSize + 1;
        assert(nArgvBufferSize <= COMMAND_MAX_LENGTH);
    }

    return true;
}

const char* CCommand::FindArg(const char* name) const
{
    int nArgC = ArgC();

    for (int i = 1; i < nArgC; i++) 
    {
        if (!_stricmp(Arg(i), name))
            return (i + 1) < nArgC ? Arg(i + 1) : "";
    }
    return 0;
}

int CCommand::FindArgInt(const char* name, int nDefaultVal) const
{
    const char* pVal = FindArg(name);
    if (pVal)
        return atoi(pVal);
    else
        return nDefaultVal;
}

int DefaultCompletionFunc(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
    return 0;
}

ConCommand::ConCommand(const char* name, FnCommandCallbackV1_t callback, const char* helpString, int flags, FnCommandCompletionCallback completionFunc)
{
    m_fnCommandCallbackV1 = callback;
    m_bUsingNewCommandCallback = false;
    m_bUsingCommandCallbackInterface = false;
    m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
    m_bHasCompletionCallback = completionFunc != 0 ? true : false;

    BaseClass::Create(name, helpString, flags);
}

ConCommand::ConCommand(const char* name, FnCommandCallback_t callback, const char* helpString, int flags, FnCommandCompletionCallback completionFunc)
{
    m_fnCommandCallback = callback;
    m_bUsingNewCommandCallback = true;
    m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
    m_bHasCompletionCallback = completionFunc != 0 ? true : false;
    m_bUsingCommandCallbackInterface = false;

    BaseClass::Create(name, helpString, flags);
}

ConCommand::ConCommand(const char* name, ICommandCallback* callback, const char* helpString, int flags, ICommandCompletionCallback* completionCallback)
{
    m_pCommandCallback = callback;
    m_bUsingNewCommandCallback = false;
    m_pCommandCompletionCallback = completionCallback;
    m_bHasCompletionCallback = (completionCallback != 0);
    m_bUsingCommandCallbackInterface = true;

    BaseClass::Create(name, helpString, flags);
}

ConCommand::~ConCommand(void)
{
}

bool ConCommand::IsCommand(void) const
{
    return true;
}

void ConCommand::Dispatch(const CCommand& command)
{
    if (m_bUsingNewCommandCallback)
    {
        if (m_fnCommandCallback) 
        {
            (*m_fnCommandCallback)(command);
            return;
        }
    }
    else if (m_bUsingCommandCallbackInterface) 
    {
        if (m_pCommandCallback) 
        {
            m_pCommandCallback->CommandCallback(command);
            return;
        }
    }
    else 
    {
        if (m_fnCommandCallbackV1) 
        {
            (*m_fnCommandCallbackV1)();
            return;
        }
    }
}

int	ConCommand::AutoCompleteSuggest(const char* partial, CUtlVector< CUtlString >& commands)
{
    if (m_bUsingCommandCallbackInterface) 
    {
        if (!m_pCommandCompletionCallback)
            return 0;
        return m_pCommandCompletionCallback->CommandCompletionCallback(partial, commands);
    }

    if (!m_fnCompletionCallback)
        return 0;

    char rgpchCommands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH];
    int iret = (m_fnCompletionCallback)(partial, rgpchCommands);

    for (int i = 0; i < iret; ++i) 
    {
        CUtlString str = rgpchCommands[i];
        commands.AddToTail(str);
    }
    return iret;
}

bool ConCommand::CanAutoComplete(void)
{
    return m_bHasCompletionCallback;
}

ConVar::ConVar(const char* name, const char* defaultValue, int flags)
{
    Create(name, defaultValue, flags);
}

ConVar::ConVar(const char* name, const char* defaultValue, int flags, const char* helpString)
{
    Create(name, defaultValue, flags, helpString);
}

ConVar::ConVar(const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax)
{
    Create(name, defaultValue, flags, helpString, bMin, fMin, bMax, fMax);
}

ConVar::ConVar(const char* name, const char* defaultValue, int flags, const char* helpString, FnChangeCallback_t callback)
{
    Create(name, defaultValue, flags, helpString, false, 0.0, false, 0.0, callback);
}

ConVar::ConVar(const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
{
    Create(name, defaultValue, flags, helpString, bMin, fMin, bMax, fMax, callback);
}

ConVar::~ConVar(void)
{
    if (m_Value.m_pszString) 
    {
        delete[] m_Value.m_pszString;
        m_Value.m_pszString = NULL;
    }
}

void ConVar::InstallChangeCallback(FnChangeCallback_t callback, bool invoke)
{
    if (callback) 
    {
        if (m_fnChangeCallbacks.GetOffset(callback) != -1) 
        {
            m_fnChangeCallbacks.AddToTail(callback);
            if (invoke)
                callback(this, m_Value.m_pszString, m_Value.m_fValue);
        }
        else 
        {
        }
    }
    else 
    {
    }
}

bool ConVar::IsFlagSet(int flag) const
{
    return (flag & m_pParent->m_nFlags) ? true : false;
}

const char* ConVar::GetHelpText(void) const
{
    return m_pParent->m_pszHelpString;
}

void ConVar::AddFlags(int flags)
{
    m_pParent->m_nFlags |= flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
    m_pParent->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif
}

int ConVar::GetFlags(void) const
{
    return m_pParent->m_nFlags;
}

bool ConVar::IsRegistered(void) const
{
    return m_pParent->m_bRegistered;
}

const char* ConVar::GetName(void) const
{
    return m_pParent->m_pszName;
}

bool ConVar::IsCommand(void) const
{
    return false;
}

void ConVar::Init()
{
    BaseClass::Init();
}

const char* ConVar::GetBaseName(void) const
{
    return m_pParent->m_pszName;
}

int ConVar::GetSplitScreenPlayerSlot(void) const
{
    return 0;
}

void ConVar::InternalSetValue(const char* value)
{
    float newValue;
    char  tempVal[32];
    char* val;

    auto temp = *(uint32_t*)&m_Value.m_fValue ^ (uint32_t)this;
    float oldValue = *(float*)(&temp);

    val = (char*)value;
    newValue = (float)atof(value);

    if (ClampValue(newValue)) {
        snprintf(tempVal, sizeof(tempVal), "%f", newValue);
        val = tempVal;
    }

    // Redetermine value
    *(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&newValue ^ (uint32_t)this;
    *(uint32_t*)&m_Value.m_nValue = (uint32_t)newValue ^ (uint32_t)this;

    if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
        ChangeStringValue(val, oldValue);
    }
}

void ConVar::ChangeStringValue(const char* tempVal, float oldValue)
{
    char* pszOldValue = (char*)stackalloc(m_Value.m_StringLength);
    memcpy(pszOldValue, m_Value.m_pszString, m_Value.m_StringLength);

    int len = strlen(tempVal) + 1;

    if (len > m_Value.m_StringLength) {
        if (m_Value.m_pszString) {
            delete[] m_Value.m_pszString;
        }

        m_Value.m_pszString = new char[len];
        m_Value.m_StringLength = len;
    }

    memcpy(m_Value.m_pszString, std::to_string(this->GetFloat()).c_str(), len);

    // Invoke any necessary callback function
    for (int i = 0; i < m_fnChangeCallbacks.Count(); i++) {
        m_fnChangeCallbacks[i](this, pszOldValue, oldValue);
    }

    if (g_CVar)
        g_CVar->CallGlobalChangeCallbacks(this, pszOldValue, oldValue);
}

bool ConVar::ClampValue(float& value)
{
    if (m_bHasMin && (value < m_fMinVal)) {
        value = m_fMinVal;
        return true;
    }

    if (m_bHasMax && (value > m_fMaxVal)) {
        value = m_fMaxVal;
        return true;
    }

    return false;
}

void ConVar::InternalSetFloatValue(float newValue)
{
    if (newValue == m_Value.m_fValue)
        return;

    ClampValue(newValue);

    // Redetermine value
    float oldValue = m_Value.m_fValue;
    *(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&newValue ^ (uint32_t)this;
    *(uint32_t*)&m_Value.m_nValue = (uint32_t)newValue ^ (uint32_t)this;

    if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
        char tempVal[32];
        snprintf(tempVal, sizeof(tempVal), "%f", m_Value.m_fValue);
        ChangeStringValue(tempVal, oldValue);
    }
    else {
        //assert(m_fnChangeCallbacks.Count() == 0);
    }
}

void ConVar::InternalSetIntValue(int value)
{
    if (value == ((int)m_Value.m_nValue ^ (int)this))
        return;

    float fValue = (float)value;
    if (ClampValue(fValue)) 
    {
        value = (int)(fValue);
    }

    // Redetermine value
    float oldValue = m_Value.m_fValue;
    *(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fValue ^ (uint32_t)this;
    *(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&value ^ (uint32_t)this;

    if (!(m_nFlags & FCVAR_NEVER_AS_STRING)) 
    {
        char tempVal[32];
        snprintf(tempVal, sizeof(tempVal), "%d", m_Value.m_nValue);
        ChangeStringValue(tempVal, oldValue);
    }
    else 
    {
    }
}

void ConVar::InternalSetColorValue(Color cValue)
{
    int color = (int)cValue.GetRawColor();
    InternalSetIntValue(color);
}

void ConVar::Create(const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
{
    static const char* emptyString = "";

    m_pParent = this;

    m_pszDefaultValue = defaultValue ? defaultValue : emptyString;

    m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
    m_Value.m_pszString = new char[m_Value.m_StringLength];
    memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

    m_bHasMin = bMin;
    m_fMinVal = fMin;
    m_bHasMax = bMax;
    m_fMaxVal = fMax;

    if (callback)
        m_fnChangeCallbacks.AddToTail(callback);

    float value = (float)atof(m_Value.m_pszString);

    *(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&value ^ (uint32_t)this;
    *(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&value ^ (uint32_t)this;

    BaseClass::Create(name, helpString, flags);
}

void ConVar::SetValue(const char* value)
{
    ConVar* var = (ConVar*)m_pParent;
    var->InternalSetValue(value);
}

void ConVar::SetValue(float value)
{
    ConVar* var = (ConVar*)m_pParent;
    var->InternalSetFloatValue(value);
}

void ConVar::SetValue(int value)
{
    ConVar* var = (ConVar*)m_pParent;
    var->InternalSetIntValue(value);
}

void ConVar::SetValue(Color value)
{
    ConVar* var = (ConVar*)m_pParent;
    var->InternalSetColorValue(value);
}

void ConVar::Revert(void)
{
    ConVar* var = (ConVar*)m_pParent;
    var->SetValue(var->m_pszDefaultValue);
}

bool ConVar::GetMin(float& minVal) const
{
    minVal = m_pParent->m_fMinVal;
    return m_pParent->m_bHasMin;
}

bool ConVar::GetMax(float& maxVal) const
{
    maxVal = m_pParent->m_fMaxVal;
    return m_pParent->m_bHasMax;
}

const char* ConVar::GetDefault(void) const
{
    return m_pParent->m_pszDefaultValue;
}