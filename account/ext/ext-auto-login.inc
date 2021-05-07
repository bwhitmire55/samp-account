//
// ext-auto-login.inc
//

#if defined EXT_AUTO_LOGIN_INC
    #endinput
#endif
#define EXT_AUTO_LOGIN_INC

static 
    DB: gDBHandle,
    gPlayerRegIp[MAX_PLAYERS][16];

public OnGameModeInit() {

    gDBHandle = db_open(ACCOUNT_DATABASE);
    if(gDBHandle == DB:0) {
        printf("[ERROR - samp-account/ext-auto-login.inc] Could not connect to the database '%s'", ACCOUNT_DATABASE);
    } else {
        Account_AddPlayerData("reg_ip", TYPE_STRING, gPlayerRegIp);
    }

    return CallLocalFunction("acce_al_OnGameModeInit", "", "");
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit 
#else 
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit acce_al_OnGameModeInit
forward acce_al_OnGameModeInit();

public OnGameModeExit() {
    if(gDBHandle != DB:0) {
        db_close(gDBHandle);
    }

    return CallLocalFunction("acce_al_OnGameModeExit", "", "");
}

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else 
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit acce_al_OnGameModeExit
forward acce_al_OnGameModeExit();

public OnPlayerConnect(playerid) {

    new uid;
    if((uid = Account_GetPlayerUID(playerid)) != 0) {
        
        new 
            query[128], 
            DBResult: result,
            ip[16];

        GetPlayerIp(playerid, ip, sizeof(ip));
        format(query, sizeof(query), "SELECT id FROM `"ACCOUNT_DATABASE_TABLE"` WHERE id = %i AND reg_ip = '%s' LIMIT 1", uid, ip);
        result = db_query(gDBHandle, query);

        if(result) {
            if(db_num_rows(result)) {
                CallLocalFunction("Account_LoadPlayerData", "i", playerid);
            }
            db_free_result(result);
        }
    }
    return CallLocalFunction("acce_al_OnPlayerConnect", "i", playerid);
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else 
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect acce_al_OnPlayerConnect
forward acce_al_OnPlayerConnect(playerid);

public OnPlayerRegister(playerid) {
    GetPlayerIp(playerid, gPlayerRegIp[playerid], 16);
    return CallLocalFunction("acce_al_OnPlayerRegister", "i", playerid);
}

#if defined _ALS_OnPlayerRegister
    #undef OnPlayerRegister
#else 
    #define _ALS_OnPlayerRegister
#endif
#define OnPlayerRegister acce_al_OnPlayerRegister
forward acce_al_OnPlayerRegister(playerid);

stock Account_GetPlayerRegistrationIp(playerid, buffer[], len = sizeof(buffer)) {
    buffer[0] = EOS;
    strcat(buffer, gPlayerRegIp[playerid], len);
}