#pragma once

#include <rehlds_api.h>

extern IRehldsApi* g_RehldsApi;
extern const RehldsFuncs_t* g_RehldsFuncs;
extern IRehldsHookchains* g_RehldsHookchains;
extern IRehldsServerStatic* g_RehldsSvs;
extern IRehldsServerData* g_RehldsData;
extern IMessageManager* g_RehldsMessageManager;

extern bool meta_init_rehlds_api();
