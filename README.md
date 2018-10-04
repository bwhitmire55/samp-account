# samp-account

[![sampctl](https://shields.southcla.ws/badge/sampctl-samp--account-2f2f2f.svg?style=for-the-badge)](https://github.com/bwhitmire55/samp-account)

samp-account was created to allow extensive user-account systems to be streamlined by not worrying about implementation details. This means we can have a fully working user account system, with data loaded from a database, and stored to a database, all with one function call.

samp-account uses the SA:MP native SQLite database system for storage, Slice's pointers library for data binding, and Y_Less' YSI hooks library for callback hooking.

## Installation

Simply install to your project:

```bash
sampctl package install bwhitmire55/samp-account
```

Include in your code and begin using the library:

```pawn
#include <account>
```

## Functions

```pawn
/*
PARAMS:  
name - The name of the database column to store the data  
type - The psuedo-type of the data (TYPE_INT, TYPE_FLOAT, TYPE_STRING)  
{Float,_}:... - The variable to store the data  
  
RETURNS:  
1 on success, otherwise 0  
*/
stock AddAccountData(const name[ACCOUNT_MAX_COLUMN_NAME], Types: type, {Float,_}:...)
```

```pawn
/*
PARAMS:  
playerid - The playerid attempting to be registered  
password - The password of the player (in plain text)  
  
RETURNS:  
1 on success, otherwise 0  
*/
stock RegisterPlayer(playerid, const password[]) 
```

```pawn
/*
PARAMS:  
playerid - The playerid attempting to be logged in  
password - The password of the player (in plain text)  
  
RETURNS:  
1 on success, otherwise 0 
*/
stock LoginPlayer(playerid, const password[])
```

```pawn
/*
PARAMS:  
playerid - The playerid to check  
  
RETURNS:  
1 (true) if logged-in, otherwise 0 (false) 
*/
bool: IsPlayerLoggedIn(playerid)
```
  
```pawn
/*
PARAMS:  
playerid - The playerid to check  
  
RETURNS:  
The unique-ID of the player in the database if exists, otherwise 0  
*/
stock GetPlayerUID(playerid)
```

## Usage

Simply create variables in which to store your players' data

```pawn
new
    gPlayerKills[MAX_PLAYERS],
    gPlayerHealth[MAX_PLAYERS],
    gPlayerNickname[MAX_PLAYERS][MAX_PLAYER_NAME];
```

Add that data to the system (and database) via AddAccountData

```pawn
public OnGameModeInit() {
    AddAccountData("kills", TYPE_INT, gPlayerKills);
    AddAccountData("health", TYPE_FLOAT, gPlayerHealth);
    AddAccountData("nickname", TYPE_STRING, gPlayerNickname);

    return 1;
}
```

Anytime a user logs into their account, their information will be loaded from the database and into the corresponding variables. Likewise for disconnecting, their data will be updated inside the database.

NOTE: The variables do no reset themselves, so you should zero-out their values upon the player exiting the server.

You are free to use the variables as normal with no effect:

```pawn
public OnPlayerDeath(playerid, killerid, reason) {
    gPlayerDeaths[playerid]++;
    return 1;
}
```

Now we just need to call RegisterPlayer or LoginPlayer. This will most likely be done via command/dialog
```pawn
ZCMD:register(playerid, params[]) {
    if(IsPlayerLoggedIn(playerid)) {
        return SendClientMessage(playerid, 0xFF0000FF, "Already logged-in!");
    }

    if(isnull(params)) {
        return SendClientMessage(playerid, 0xFFFF00FF, "Usage: /register <password>");
    }

    if(RegisterPlayer(playerid, params)) {
        SendClientMessage(playerid, 0x00FF00FF, "You have successfully registered an account!");
    } else {
        // RegisterPlayer will return 0 if the account already exists, or there is an issue with the database.
        // For this example, we'll assume the former.
        SendClientMessage(playerid, 0xFF0000FF, "Error! This username is already registered.");
    }
    return 1;
}
```

And that's it! You now have a fully working account system, which can store any data you like, without touching a database or file. Nice!

## Callbacks

This library also includes two callbacks, OnPlayerRegister and OnPlayerLogin.

```pawn
public OnPlayerRegister(playerid) {
    SendClientMessageToAll(0x00FF00FF, "A new member has registered!");
    return 1;
}

public OnPlayerLogin(playerid) {
    SendClientMessageToAll(0x00FF00FF, "An existing member has rejoined us!");
    return 1;
}
```

## Macros

All macros are as followed:

```pawn
// The database file
#define ACCOUNT_DATABASE        "mydatabase.db"
```

```pawn
// The database table to store the account data
#define ACCOUNT_DATABASE_TABLE  "mydatabasetable"
```

```pawn
// The amount of 'data' you wish to store
// i.e., how many times you will use AddAccountData
#define ACCOUNT_MAX_COLUMNS     (100)
```

```pawn
// The maximum length of a column name in the database
#define ACCOUNT_MAX_COLUMN_NAME (24)
```

All of these can be redefined to suite your script
```pawn
#define ACCOUNT_DATABASE        "COD-DB.db"
#define ACCOUNT_DATABASE_TABLE  "users"
#include <account>
```

## Improvements / Updates

In the future, I may extend this to optionally include predefined commands or dialogs for the user accounts. 

## Testing

To test, simply run the package:

```bash
sampctl package run
```
