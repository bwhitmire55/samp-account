# samp-account

[![sampctl](https://shields.southcla.ws/badge/sampctl-samp--account-2f2f2f.svg?style=for-the-badge)](https://github.com/bwhitmire55/samp-account)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->
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
stock AddAccountData(const name[ACCOUNT_MAX_COLUMN_NAME], Types: type, {Float,_}:...)
```
PARAMS:
name - The name of the database column to store the data
type - The psuedo-type of the data (TYPE_INT, TYPE_FLOAT, TYPE_STRING)
{Float,_}:... - The variable to store the data

RETURNS:
1 on success, otherwise 0

```pawn
stock RegisterPlayer(playerid, const password[]) 
```
PARAMS:
playerid - The playerid attempting to be registered
password - The password of the player (in plain text)

RETURNS:
1 on success, otherwise 0

```pawn
stock LoginPlayer(playerid, const password[])
```
PARAMS:
playerid - The playerid attempting to be logged in
password - The password of the player (in plain text)

RETURNS:
1 on success, otherwise 0

```pawn
bool: IsPlayerLoggedIn(playerid)
```
PARAMS:
playerid - The playerid to check

RETURNS:
1 (true) if logged-in, otherwise 0 (false) 

```pawn
stock GetPlayerUID(playerid) {
    return gPlayerData[playerid][eUID];
}
```
PARAMS:
playerid - The playerid to check

RETURNS:
The unique-ID of the player in the database if exists, otherwise 0

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

And that's it! Anytime a user logs into their account, their information will be loaded from the database and into the corresponding variables. Likewise for disconnecting, their data will be updated inside the database.

NOTE: The variables do no reset themselves, so you should zero-out their values upon the player exiting the server.

You are free to use the variables as normal with no effect:

```pawn
public OnPlayerDeath(playerid, killerid, reason) {
    gPlayerDeaths[playerid]++;
    return 1;
}
```

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

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
