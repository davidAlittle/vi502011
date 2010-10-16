//
// PluginExample.cpp : Defines the initialization routines for the plugin DLL.
//
#include "CKAll.h"
#include "ManagerExample.h"
#include "BB_InitHaptic.h"
#include <windows.h>
#include <winbase.h>

//------------------------------
// Public Functions
//------------------------------

// Invoked when DLL may be initialized
CKERROR InitInstance(CKContext* context);

// Invoked when DLL may be released
CKERROR ExitInstance(CKContext* context);

// Invoked to register Virtools Behaviours from inside this plugin
PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);

// Invoked to retreive the count of components in this plugin.
PLUGIN_EXPORT int CKGetPluginInfoCount();

// Invoked to retreive the plugin's informations at the given index.
PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int index);

//------------------------------
// DLL Variables
//------------------------------

// manager
static ManagerExample* theManager;

// Information about this Virtools plugin
static CKPluginInfo g_PluginInfo;


//----------------------------------
// Public Function Implementations
//----------------------------------

CKERROR InitInstance(CKContext* context)
{
	CKParameterManager* pm = context->GetParameterManager();

	//TODO: Launch manager's threads here
	theManager = new ManagerExample(context);

	return CK_OK;
}

CKERROR ExitInstance(CKContext* context)
{
	CKParameterManager* pm = context->GetParameterManager();

	//TODO: Do some cleaning here
	delete theManager;

	return CK_OK;
}

void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{
	//TODO: Add declaration feedback functions for your BB
	RegisterBehavior(reg, FillBehaviorBB_InitHapticDecl);
}

PLUGIN_EXPORT int CKGetPluginInfoCount()
{
	return 1;
}

PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	//TODO: Update following informations according to your interests
	g_PluginInfo.m_Author			= AUTHOR_NAME;
	g_PluginInfo.m_Description		= "Some comments";
	g_PluginInfo.m_Extension		= "";
	g_PluginInfo.m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo.m_Version			= BB_VERSION;
	g_PluginInfo.m_InitInstanceFct	= InitInstance;
	g_PluginInfo.m_ExitInstanceFct	= ExitInstance;
	g_PluginInfo.m_GUID				= CKGUID(0x01ddd3ef,0x01ddd3df);
	g_PluginInfo.m_Summary			= BB_CATEGORY;
	return &g_PluginInfo;
}
