#include <extdll.h>
#include <meta_api.h>
#include "ex_rehlds_api.h"

mutil_funcs_t *gpMetaUtilFuncs;
meta_globals_t *gpMetaGlobals;

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,			// ifvers
	"RicoChatSlot",					// name
	"0.0",							// version
	__DATE__,						// date
	"GoldSrc.one",					// author
	"https://goldsrc.one/",			// url
	"RICOSLOT",						// logtag
	PT_ANYTIME,						// (when) loadable
	PT_ANYTIME,						// (when) unloadable
};

static void ClientCommand(edict_t *pEntity)
{
	static char cmdBuf[16];
	const char *arg = CMD_ARGV(1);
	if (CMD_ARGC() == 2 && arg[0] >= '0' && arg[0] <= '9' && !arg[1]) {
		const char *cmd = CMD_ARGV(0);
		IGameClient *cl = g_RehldsSvs->GetClient(ENTINDEX(pEntity) - 1);
		if ((!strcmp(cmd, "say") || !strcmp(cmd, "say_team")) && cl && !strcmp(cl->GetGame(), "ricochet")) {
			snprintf(cmdBuf, sizeof(cmdBuf), "menuselect %d", arg[0] == '0' ? 10 : arg[0] - '0');
			g_RehldsFuncs->TokenizeString(cmdBuf);
			
			static int msgShowMenu = 0;
			if (!msgShowMenu)
				msgShowMenu = GET_USER_MSG_ID(PLID, "ShowMenu", NULL);
			if (msgShowMenu) {
				MESSAGE_BEGIN(MSG_ONE, msgShowMenu, NULL, pEntity);
				WRITE_SHORT(0);
				WRITE_CHAR(0);
				WRITE_BYTE(0);
				WRITE_STRING("");
				MESSAGE_END();
			}
		}
	}
	RETURN_META(MRES_IGNORED);
}

static int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	memset(pFunctionTable, 0, sizeof(DLL_FUNCTIONS));
	pFunctionTable->pfnClientCommand = ClientCommand;
	return TRUE;
}

C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t **plinfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*plinfo = &Plugin_info;
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return TRUE;
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	if(!meta_init_rehlds_api())
		return FALSE;

	gpMetaGlobals = pMGlobals;
	pFunctionTable->pfnGetEntityAPI2 = GetEntityAPI2;
	return TRUE;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	return TRUE;
}
