// generated by "sampctl package generate"

// hashing algorithm
#define ACCOUNT_USE_BCRYPT

// extension
//#define ACCOUNT_EXT_AUTO_LOGIN
#include	"account.inc"

new 
	gPlayerKills[MAX_PLAYERS],
	gPlayerDeaths[MAX_PLAYERS],
	Float: gPlayerHealth[MAX_PLAYERS],
	gPlayerPhrase[MAX_PLAYERS][30];

main() {
	// write tests for libraries here and run "sampctl package run"
}

public OnGameModeInit() {
	AddPlayerClass(0, 0.00, 0.00, 0.00, 0.00, 0, 0, 0, 0, 0, 0);

	Account_AddPlayerData("kills", TYPE_INT, gPlayerKills);
	Account_AddPlayerData("deaths", TYPE_INT, gPlayerDeaths);
	Account_AddPlayerData("health", TYPE_FLOAT, gPlayerHealth);
	Account_AddPlayerData("phrase", TYPE_STRING, gPlayerPhrase);
	return 1;
}

public OnPlayerRegister(playerid) {
	SendClientMessageToAll(0x00FF00FF, "A new member has joined the server!");
	return 1;
}

public OnPlayerLogin(playerid) {
	SendClientMessageToAll(0x00FF00FF, "An existing member has returned!");
	
	#if defined ACCOUNT_EXT_AUTO_LOGIN
		new ip[16], buffer[128];
		Account_GetPlayerRegistrationIp(playerid, ip, sizeof(ip));
		format(buffer, sizeof(buffer), "You registered at some point with the IP %s", ip);
		SendClientMessage(playerid, 0x00FF00FF, buffer);
	#endif
	return 1;
}

public OnPlayerFailRegistration(playerid) {
	SendClientMessage(playerid, 0xFF0000FF, "This username already exists!");
	return 1;
}

public OnPlayerInvalidLoginAttempt(playerid) {
	SendClientMessage(playerid, 0xFF0000FF, "Invalid password!");
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
	gPlayerDeaths[playerid]++;

	if(Account_IsPlayerLoggedIn(killerid)) {
		gPlayerKills[killerid]++;
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[]) {
	
	if(!strcmp(cmdtext, "/stats", true)) {
		new string[128];
		format(string, sizeof(string), "Kills: %i | Deaths: %i | Health: %0.2f, Phrase: %s",
			gPlayerKills[playerid],
			gPlayerDeaths[playerid],
			gPlayerHealth[playerid],
			gPlayerPhrase[playerid]
		);
		SendClientMessage(playerid, 0x00FF00FF, string);
		return 1;
	}

	if(!strcmp(cmdtext, "/register", true, 9)) {
		if(Account_IsPlayerLoggedIn(playerid)) 
			return SendClientMessage(playerid, 0xFF0000FF, "Already logged in!");

		if(!strlen(cmdtext[10])) {
			return SendClientMessage(playerid, 0xFF0000FF, "Usage: /register <password>");
		}

		Account_RegisterPlayer(playerid, cmdtext[10]);
		return 1;
	}

	if(!strcmp(cmdtext, "/login", true, 6)) {
		if(Account_IsPlayerLoggedIn(playerid))
			return SendClientMessage(playerid, 0xFF0000FF, "Already logged in!");

		if(!strlen(cmdtext[7])) {
			return SendClientMessage(playerid, 0xFF0000FF, "Usage: /login <password>");
		}

		Account_LoginPlayer(playerid, cmdtext[7]);
		return 1;
	}

	if(!strcmp(cmdtext, "/savekills", true)) {
		Account_UpdatePlayerData(playerid, gPlayerKills);
		return 1;
	}

	if(!strcmp(cmdtext, "/savealot", true)) {
		Account_UpdatePlayerData(playerid, gPlayerKills, gPlayerHealth, gPlayerPhrase, gPlayerDeaths);
		return 1;
	}
	return 0;
}