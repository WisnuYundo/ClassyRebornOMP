											// CLASSY COUNTRY ROLEPLAY 
											// VERSI		:	0.1 BETA
											// DIBUAT OLEH	: HANSALEXANDER/WISNUYUNDO
											// Powered by	: Open.MP  
// CHANGELOG VERSI 0.1 BETA:
// -Login Register System User Control Panel 
// -Pembuatan 3 Character dalam satu UCP
// -Penambahan System Saving dan Load menggunakan Plugin MySQL Terbaru



#define YSI_YES_HEAP_MALLOC
#include <open.mp>
#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <easyDialog>
#include <samp_bcrypt>

#define FOREACH_NO_LOCALS
#define FOREACH_NO_BOTS
#define FOREACH_NO_ACTORS
#define FOREACH_NO_STREAMED
#include <YSI_Coding\y_va>
#include <YSI_Server\y_colours>
#include <YSI_Coding\y_stringhash> // Performs compile-time hashing of strings, for use as array indexes, case values, and more.
#include <YSI_Coding\y_timers>	   // SetTimer and SetTimerEx wrappers with task and ptask
#include <YSI_Data\y_iterate>	   // The latest version of foreach with many extras for iterators and special iterators (iterator functions).


#include "modular/database/var.inc"
#include "modular/database/function.inc"


#include "modular/server/define.inc"
#include "modular/server/var.inc"
#include "modular/server/function.inc"

#include "modular/player/var.inc"
#include "modular/player/function.inc"
/*
     ___      _
    / __| ___| |_ _  _ _ __
    \__ \/ -_)  _| || | '_ \
    |___/\___|\__|\_,_| .__/
                      |_|
*/

main()
{
	printf(" ");
	printf("  -------------------------------");
	printf("  |  Classy Open.MP Gamemode |");
	printf("  -------------------------------");
	printf(" ");
}

public OnGameModeInit()
{
	OnMySQLConnect();
	SetupPlayerTable();
	return 1;
}

public OnGameModeExit()
{
	mysql_close(g_SQL);
	return 1;
}

/*
      ___
     / __|___ _ __  _ __  ___ _ _
    | (__/ _ \ '  \| '  \/ _ \ ' \
     \___\___/_|_|_|_|_|_\___/_||_|

*/

public OnPlayerConnect(playerid)
{
	ResetAccountVariable(playerid);
	ResetCharacterVariable(playerid);
	GetPlayerName(playerid, UcpData[playerid][UcpName], MAX_PLAYER_NAME);

	// send a query to recieve all the stored player data from the table
	new query[124];
	mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `players` WHERE `username` = '%e' LIMIT 1", UcpData[playerid][UcpName]);
	mysql_tquery(g_SQL, query, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	g_MysqlRaceCheck[playerid]++;
	UpdatePlayerData(playerid);
	if (cache_is_valid(UcpData[playerid][Cache_ID]))
	{
		cache_delete(UcpData[playerid][Cache_ID]);
		UcpData[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
	}

	// if the player was kicked before the time expires (30 seconds), kill the timer
	if (UcpData[playerid][LoginTimer])
	{
		KillTimer(UcpData[playerid][LoginTimer]);
		UcpData[playerid][LoginTimer] = 0;
	}

	// sets "IsLoggedIn" to false when the player disconnects, it prevents from saving the player data twice when "gmx" is used
	UcpData[playerid][IsLoggedIn] = false;
	return 1;
}
/*
public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 217.8511, -98.4865, 1005.2578);
	SetPlayerFacingAngle(playerid, 113.8861);
	SetPlayerInterior(playerid, 15);
	SetPlayerCameraPos(playerid, 215.2182, -99.5546, 1006.4);
	SetPlayerCameraLookAt(playerid, 217.8511, -98.4865, 1005.2578);
	ApplyAnimation(playerid, "benchpress", "gym_bp_celebrate", 4.1, true, false, false, false, 0, SYNC_NONE);
	return 1;
}*/

public OnPlayerSpawn(playerid)
{
	if(!pInfo[playerid][pSpawned])
	{
		pInfo[playerid][pSpawned] = true;
		SetPlayerScore(playerid, pInfo[playerid][pLevel]);
		GivePlayerMoney(playerid, pInfo[playerid][pMoney]);
		SetPlayerHealth(playerid, pInfo[playerid][pHealth]);
		SetPlayerArmour(playerid, pInfo[playerid][pArmour]);
		SetPlayerVirtualWorld(playerid, pInfo[playerid][pWorld]);
		SetPlayerInterior(playerid, pInfo[playerid][pInterior]);
		new Float:x, Float:y, Float:z, Float:a;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);
		SetPlayerPos(playerid, x, y, z);
		SetPlayerFacingAngle(playerid, a);
		SetCameraBehindPlayer(playerid);
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

/*
     ___              _      _ _    _
    / __|_ __  ___ __(_)__ _| (_)__| |_
    \__ \ '_ \/ -_) _| / _` | | (_-<  _|
    |___/ .__/\___\__|_\__,_|_|_/__/\__|
        |_|
*/


public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, weaponid, bodypart)
{
	return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
	return 1;
}

public OnActorStreamOut(actorid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case DIALOG_UNUSED: return 1; // Useful for dialogs that contain only information and we do nothing depending on whether they responded or not

		case DIALOG_LOGIN:
		{
		    if(!response)
		        return Kick(playerid);
		        
	        if(strlen(inputtext) < 1)
	        {
				new str[256];
	            format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", UcpData[playerid][UcpName], UcpData[playerid][LoginAttempts]);
	            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Classy Country", str, "Login", "Exit");
	            return 1;
			}
			new pwQuery[256], hash[BCRYPT_HASH_LENGTH];
			mysql_format(g_SQL, pwQuery, sizeof(pwQuery), "SELECT password FROM players WHERE username = '%e' LIMIT 1", UcpData[playerid][UcpName]);
			mysql_query(g_SQL, pwQuery);
			
	        cache_get_value_name(0, "password", hash, sizeof(hash));
	        
	        bcrypt_verify(playerid, "OnPlayerPasswordChecked", inputtext, hash);

		}
		case DIALOG_REGISTER:
		{
			if(!response)
		        return Kick(playerid);
			new str[256];
		    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Create Password: {FF00FF}(Input Below)", UcpData[playerid][UcpName], UcpData[playerid][LoginAttempts]);

	        if(strlen(inputtext) < 7)
				return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Xyronite", str, "Register", "Exit");

	        if(strlen(inputtext) > 32)
				return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Xyronite", str, "Register", "Exit");

	        bcrypt_hash(playerid, "HashPlayerPassword", inputtext, BCRYPT_COST);
		}
		case DIALOG_CHARLIST:
		{
			if(response)
			{
				if (PlayerChar[playerid][listitem][0] == EOS)
					return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Exit");

				pInfo[playerid][pName] = listitem;
				SetPlayerName(playerid, PlayerChar[playerid][listitem]);

				new cQuery[256];
				mysql_format(g_SQL, cQuery, sizeof(cQuery), "SELECT * FROM `characters` WHERE `name` = '%s' LIMIT 1;", PlayerChar[playerid][pInfo[playerid][pName]]);
				mysql_tquery(g_SQL, cQuery, "AssignPlayerData", "d", playerid);
				
			}
		}
		case DIALOG_MAKECHAR:
		{
		    if(response)
		    {
			    if(strlen(inputtext) < 1 || strlen(inputtext) > 24)
					return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");

				if(!strfind(inputtext, "_", true))
					return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");

				new characterQuery[178];
				mysql_format(g_SQL, characterQuery, sizeof(characterQuery), "SELECT * FROM `characters` WHERE `name` = '%s'", inputtext);
				mysql_tquery(g_SQL, characterQuery, "InsertPlayerName", "ds", playerid, inputtext);

			    format(pInfo[playerid][pUCP], 22, GetName(playerid));
			}
		}
		default: return 0; // dialog ID was not found, search in other scripts
	}
	return 1;
}

public OnPlayerEnterGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeaveGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerEnterPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeavePlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
	return 1;
}

public OnPlayerRequestDownload(playerid, DOWNLOAD_REQUEST:type, crc)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 0;
}

public OnPlayerSelectObject(playerid, SELECT_OBJECT:type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:rotationX, Float:rotationY, Float:rotationZ)
{
	return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:rotationX, Float:rotationY, Float:rotationZ, Float:scaleX, Float:scaleY, Float:scaleZ)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
	return 1;
}

public OnPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnScriptCash(playerid, amount, source)
{
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnIncomingConnection(playerid, ip_address[], port)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, component)
{
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjob)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, colour1, colour2)
{
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	return 1;
}

