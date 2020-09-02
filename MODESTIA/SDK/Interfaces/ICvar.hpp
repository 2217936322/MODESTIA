#pragma once

#include "IAppSystem.hpp"
#include "IConVar.hpp"

class ConCommandBase;
class ConCommand;
class ConVar;

typedef int CVarDLLIdentifier_t;

class IConsoleDisplayFunc
{
public:
    virtual void ColorPrint(const uint8_t* color, const char* message) = 0;
    virtual void Print(const char* message) = 0;
    virtual void DPrint(const char* message) = 0;
};

class ICvar : public IAppSystem
{
public:
    virtual CVarDLLIdentifier_t        AllocateDLLIdentifier() = 0;
    virtual void                       RegisterConCommand(ConCommandBase* commandBase) = 0;
    virtual void                       UnregisterConCommand(ConCommandBase* commandBase) = 0;
    virtual void                       UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
    virtual const char*                GetCommandLineValue(const char* varName) = 0;
    virtual ConCommandBase*            FindCommandBase(const char* name) = 0;
    virtual const ConCommandBase*      FindCommandBase(const char* name) const = 0;
    virtual ConVar*                    FindVar(const char* varName) = 0;
    virtual const ConVar*              FindVar(const char* varName) const = 0;
    virtual ConCommand*                FindCommand(const char* name) = 0;
    virtual const ConCommand*          FindCommand(const char* name) const = 0;
    virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
    virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
    virtual void                       CallGlobalChangeCallbacks(ConVar* var, const char* oldString, float oldValue) = 0;
    virtual void                       InstallConsoleDisplayFunc(IConsoleDisplayFunc* displayFunc) = 0;
    virtual void                       RemoveConsoleDisplayFunc(IConsoleDisplayFunc* displayFunc) = 0;
    virtual void                       ConsoleColorPrintf(const uint8_t* color, const char* format, ...) const = 0;
    virtual void                       ConsolePrintf(const char* format, ...) const = 0;
    virtual void                       ConsoleDPrintf(const char* format, ...) const = 0;
    virtual void                       RevertFlaggedConVars(int flag) = 0;
};
