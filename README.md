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

## Installation

Simply install to your project:

```bash
sampctl package install bwhitmire55/samp-account
```

Include in your code and begin using the library:

```pawn
#include <account>
```

## Usage

Simply create variables in which to store your players' data

```pawn
new
    gPlayerKills[MAX_PLAYERS],
    gPlayerHealth[MAX_PLAYERS],
    gPlayerNickname[MAX_PLAYERS];
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

And that's it! The data for logged-in users is automatically retrieved from, or loaded to, those variables upon
the player logging-in or leaving the server. You can manipulate the variables in whatever way you see fit; i.e.

```pawn
public OnPlayerDeath(playerid, killerid, reason) {
    gPlayerDeaths[playerid]++;
    return 1;
}
```

And the correct value will be saved to the database. And also loaded from the database, to the variable, upon login.

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
