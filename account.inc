//
// account.inc
//

#if defined ACCOUNT_INC 
    #endinput 
#endif 
#define ACCOUNT_INC 

#include <a_samp>
#include <pointers>
#include <YSI\y_hooks>

native WP_Hash(buffer[], len, const str[]);

#if !defined ACCOUNT_DATABASE 
    #define ACCOUNT_DATABASE            "accounts.db"
#endif 

#if !defined ACCOUNT_DATABASE_TABLE 
    #define ACCOUNT_DATABASE_TABLE      "accounts"
#endif

#if !defined ACCOUNT_MAX_COLUMNS 
    #define ACCOUNT_MAX_COLUMNS         (20)
#endif 

#if !defined ACCOUNT_MAX_COLUMN_NAME 
    #define ACCOUNT_MAX_COLUMN_NAME     (20)
#endif

/*
#if defined ACCOUNT_PASSWORD_MIN || defined ACCOUNT_PASSWORD_MAX
    #if !defined ACCOUNT_PASSWORD_MIN || !defined ACCOUNT_PASSWORD_MAX
        #error "Must define both 'ACCOUNT_PASSWORD_MIN' and 'ACCOUNT_PASSWORD_MAX' together"
    #endif
#endif 

#if !defined ACCOUNT_USE_DIALOGS && !defined ACCOUNT_USE_CMDS 
    #error "Must define either 'ACCOUNT_USE_DIALOGS' or 'ACCOUNT_USE_COMMANDS' (or both) for account registration"
#endif 
*/
enum Types 
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
};

enum ePlayer 
{
    eUID,
    bool: bLoggedIn
};

enum eAccount 
{
    eName[ACCOUNT_MAX_COLUMN_NAME],
    Types: eType,
    eAddress
};

static 
    gPlayerData[MAX_PLAYERS][ePlayer],
    gAccountData[ACCOUNT_MAX_COLUMNS][eAccount],
    DB: gDBHandle;

hook OnGameModeInit()
{
    gDBHandle = db_open(ACCOUNT_DATABASE);

    if(!IsValidDB(gDBHandle)) {
        printf("[ERROR - account.inc] Could not connect to the database '%s'", ACCOUNT_DATABASE);
    } else {
        CreateDefaultTable();
    }
    return 1;
}

hook OnGameModeExit()
{
    if(IsValidDB(gDBHandle)) {
        db_close(gDBHandle);
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) 
{
    gPlayerData[playerid][eUID] = 0;
    gPlayerData[playerid][bLoggedIn] = false;
    return 1;
}

stock RegisterPlayer(playerid, const password[]) 
{
    if(!IsValidDB(gDBHandle)) {
        print("[ERROR - account.inc] Not connected to the database!");
        return 0;
    }

    new 
        query[256],
        DBResult: result;

    format(query, sizeof(query), "SELECT id FROM `"ACCOUNT_DATABASE_TABLE"` WHERE username = '%s'", ReturnPlayerName(playerid));
    result = db_query(gDBHandle, query);

    if(result) {
        if(db_num_rows(result)) {
            printf("[ERROR - account.inc] Username is already registered -> '%s'", ReturnPlayerName(playerid));
        } else {
            db_free_result(result);

            new pass[129], ip[16];
            WP_Hash(pass, sizeof(pass), password);
            GetPlayerIp(playerid, ip, sizeof(ip));

            format(query, sizeof(query), "INSERT INTO `"ACCOUNT_DATABASE_TABLE"` (username, password, reg_ip) VALUES('%s','%s','%s')",
                ReturnPlayerName(playerid),
                pass,
                ip
            );

            db_query(gDBHandle, query);
            format(query, sizeof(query), "SELECT id FROM `"ACCOUNT_DATABASE_TABLE"` WHERE username = '%s'", ReturnPlayerName(playerid));
            result = db_query(gDBHandle, query);

            gPlayerData[playerid][eUID] = db_get_field_assoc_int(result, "id");
            gPlayerData[playerid][bLoggedIn] = true;
            
            db_free_result(result);
            return 1;
        }
        db_free_result(result);
    }
    return 0;
}

stock LoginPlayer(playerid, const password[]) 
{
    if(!IsValidDB(gDBHandle)) {
        print("[ERROR - account.inc] Not connected to the database!");
        return 0;
    }

    new 
        query[256],
        DBResult: result,
        pass[129];

    WP_Hash(pass, sizeof(pass), password);
    format(query, sizeof(query), "SELECT id FROM `"ACCOUNT_DATABASE_TABLE"` WHERE username = '%s' AND password = '%s'",
        ReturnPlayerName(playerid),
        pass
    );

    result = db_query(gDBHandle, query);

    if(result) {
        if(!db_num_rows(result)) {
            printf("[ERROR - account.inc] Login attempt failed -> '%s'", ReturnPlayerName(playerid));
        } else {
            gPlayerData[playerid][eUID] = db_get_field_assoc_int(result, "id");
            gPlayerData[playerid][bLoggedIn] = true;
            db_free_result(result);

            // LoadAccountData()
            return 1;
        }
        db_free_result(result);
    }
    return 0;
}

stock AddAccountData(const name[ACCOUNT_MAX_COLUMN_NAME], Types: type, {Float,_}:...)
{
    if(numargs() != 3) {
        printf("[ERROR - account.inc] Invalid number of arguments passed to 'AddAccountData' (%i/3)", numargs());
        return 0;
    }

    if(!IsValidDB(gDBHandle)) {
        print("[ERROR - account.inc] Not connected to the database!");
        return 0;
    }

    new slot = GetOpenAccountSlot();

    if(slot == -1) {
        print("[ERROR - account.inc] Maximum account columns reached. Increase 'ACCOUNT_MAX_COLUMNS'");
        return 0;
    }

    strcat(gAccountData[slot][eName], name);
    gAccountData[slot][eType] = type;
    gAccountData[slot][eAddress] = GetArgumentAddress(2);

    new query[128] = "ALTER TABLE `"ACCOUNT_DATABASE_TABLE"` ADD ";

    strcat(query, name);
    strcat(query, " ");

    switch(type) {
        case TYPE_INT:
            strcat(query, "INTEGER");
        case TYPE_FLOAT:
            strcat(query, "FLOAT");
        case TYPE_STRING:
            strcat(query, "TEXT");
        default: {
            printf("[ERRROR - account.inc] Invalid type passed to 'AddAccountData' (%i)", _:type);
            return 0;
        }
    }

    db_query(gDBHandle, query);
    return 1;
}

stock bool: IsPlayerLoggedIn(playerid) {
    return gPlayerData[playerid][bLoggedIn];
}

static IsValidDB(DB: handle) {
    if(handle != DB:0) {
        return 1;
    }
    return 0;
}

static GetOpenAccountSlot() {
    for(new i = 0; i < ACCOUNT_MAX_COLUMNS; i++) {
        if(gAccountData[i][eAddress] == 0) {
            return i;
        }
    }
    return -1;
}

static CreateDefaultTable() {
    if(IsValidDB(gDBHandle)) {
        db_query(gDBHandle, "CREATE TABLE IF NOT EXISTS `"ACCOUNT_DATABASE_TABLE"` ( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            username TEXT, \
            password BLOB, \
            reg_ip TEXT \
        );");
    }
}