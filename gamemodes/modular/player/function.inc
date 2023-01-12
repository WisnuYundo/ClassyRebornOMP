stock ResetAccountVariable(playerid)
{
	g_MysqlRaceCheck[playerid]++;

	// reset player data
	static const empty_player[E_UCP];
	UcpData[playerid] = empty_player;
	return 1;
}

stock ResetCharacterVariable(playerid)
{
	pInfo[playerid][pSpawned] = false;
	pInfo[playerid][pHealth] = 100.0;
	pInfo[playerid][pArmour] = 0.0;
	pInfo[playerid][pMoney] = 500;
	pInfo[playerid][pLevel] = 1;
	pInfo[playerid][pExp] = 1;
	pInfo[playerid][pInterior] = 0;
	pInfo[playerid][pWorld] = 0;
	return 1;
}

FUNC::OnPlayerDataLoaded(playerid, race_check)
{

	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	new string[115];
	if(cache_num_rows() > 0)
	{
		cache_get_value(0, "password", UcpData[playerid][UcpPassword], 65);

		UcpData[playerid][Cache_ID] = cache_save();

		format(string, sizeof string, "This account (%s) is registered.\nPlease login by entering your password in the field below:", UcpData[playerid][UcpName]);
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "Abort");

		UcpData[playerid][LoginTimer] = SetTimerEx("OnLoginTimeout", SECONDS_TO_LOGIN * 1000, false, "d", playerid);
	}
	else
	{
		format(string, sizeof string, "Welcome %s,\nyou can register by entering your password in the field below:", UcpData[playerid][UcpName]);
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registration", string, "Register", "Abort");
	}
	return 1;
}


FUNC::OnLoginTimeout(playerid)
{
	// reset the variable that stores the timerid
	UcpData[playerid][LoginTimer] = 0;
	
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have been kicked for taking too long to login successfully to your account.", "Okay", "");
	DelayedKick(playerid);
	return 1;
}


FUNC::OnPlayerRegister(playerid)
{
	// retrieves the ID generated for an AUTO_INCREMENT column by the sent query
	UcpData[playerid][UcpID] = cache_insert_id();
	new string[128];
	format(string, sizeof string, "This account (%s) is registered.\nPlease login by entering your password in the field below:", UcpData[playerid][UcpName]);
	ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "Abort");

	UcpData[playerid][IsLoggedIn] = true;
	return 1;
}


FUNC::_KickPlayerDelayed(playerid)
{
	Kick(playerid);
	return 1;
}


//-----------------------------------------------------

FUNC::AssignPlayerData(playerid)
{
	cache_get_value_int(0, "id", pInfo[playerid][pID]);
	cache_get_value_name(0, "name", pInfo[playerid][pName], MAX_PLAYER_NAME);
	cache_get_value_name_int(0, "skin", pInfo[playerid][pSkin]);
	cache_get_value_name_int(0, "interior", pInfo[playerid][pInterior]);
	cache_get_value_name_int(0, "world", pInfo[playerid][pWorld]);
	cache_get_value_name_float(0, "x", pInfo[playerid][pPosX]);
	cache_get_value_name_float(0, "y", pInfo[playerid][pPosY]);
	cache_get_value_name_float(0, "z", pInfo[playerid][pPosZ]);
	cache_get_value_name_float(0, "a", pInfo[playerid][pPosAngle]);
	cache_get_value_name_int(0, "money", pInfo[playerid][pMoney]);
	cache_get_value_name_float(0, "health", pInfo[playerid][pHealth]);
	cache_get_value_name_float(0, "armour", pInfo[playerid][pArmour]);
	cache_get_value_name_int(0, "level", pInfo[playerid][pLevel]);
	SetSpawnInfo(playerid, NO_TEAM, pInfo[playerid][pSkin], pInfo[playerid][pPosX], pInfo[playerid][pPosY], pInfo[playerid][pPosZ], pInfo[playerid][pPosAngle]);
	SpawnPlayer(playerid);
	return 1;
}

DelayedKick(playerid, time = 500)
{
	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
	return 1;
}

SetupPlayerTable()
{
	mysql_tquery(g_SQL, "CREATE TABLE IF NOT EXISTS `players` (`id` int(11) NOT NULL AUTO_INCREMENT,`username` varchar(24) NOT NULL,`password` char(64) NOT NULL,`salt` char(16) NOT NULL,`kills` mediumint(8) NOT NULL DEFAULT '0',`deaths` mediumint(8) NOT NULL DEFAULT '0',`x` float NOT NULL DEFAULT '0',`y` float NOT NULL DEFAULT '0',`z` float NOT NULL DEFAULT '0',`angle` float NOT NULL DEFAULT '0',`interior` tinyint(3) NOT NULL DEFAULT '0', PRIMARY KEY (`id`), UNIQUE KEY `username` (`username`))");
	mysql_tquery(g_SQL, "CREATE TABLE IF NOT EXISTS `characters` (`id` int(11) NOT NULL AUTO_INCREMENT,`name` varchar(24) NOT NULL,`password` char(64) NOT NULL,`kills` mediumint(8) NOT NULL DEFAULT '0',`deaths` mediumint(8) NOT NULL DEFAULT '0',`x` float NOT NULL DEFAULT '0',`y` float NOT NULL DEFAULT '0',`z` float NOT NULL DEFAULT '0',`angle` float NOT NULL DEFAULT '0',`interior` tinyint(3) NOT NULL DEFAULT '0', PRIMARY KEY (`id`), UNIQUE KEY `name` (`name`))");
	return 1;
}



UpdatePlayerData(playerid)
{
	if (UcpData[playerid][IsLoggedIn] == false) return 0;
	GetPlayerPos(playerid, pInfo[playerid][pPosX], pInfo[playerid][pPosY], pInfo[playerid][pPosZ]);
	GetPlayerFacingAngle(playerid, pInfo[playerid][pPosAngle]);
	
	new cQuery[3048];
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "UPDATE `characters` SET ");
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`x` = '%f', ", cQuery, pInfo[playerid][pPosX]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`y` = '%f', ", cQuery, pInfo[playerid][pPosY]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`z` = '%f', ", cQuery, pInfo[playerid][pPosZ]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`a` = '%f', ", cQuery, pInfo[playerid][pPosAngle]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`skin` = '%d', ", cQuery, pInfo[playerid][pSkin]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`interior` = '%d', ", cQuery, pInfo[playerid][pInterior]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`world` = '%d', ", cQuery, pInfo[playerid][pWorld]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`money` = '%d', ", cQuery, pInfo[playerid][pMoney]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`health` = '%f', ", cQuery, pInfo[playerid][pHealth]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`armour` = '%f', ", cQuery, pInfo[playerid][pArmour]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`level` = '%d', ", cQuery, pInfo[playerid][pLevel]);
	mysql_format(g_SQL, cQuery, sizeof(cQuery), "%s`last_login` = CURRENT_TIMESTAMP() WHERE `id` = '%d'", cQuery, pInfo[playerid][pID]);
	mysql_tquery(g_SQL, cQuery);
		
	return 1;
}

FUNC::OnPlayerPasswordChecked(playerid, bool:success)
{
	new str[256];
    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", UcpData[playerid][UcpName], UcpData[playerid][LoginAttempts]);
    
	if(!success)
	{
	    if(UcpData[playerid][LoginAttempts] < 3)
	    {
		    UcpData[playerid][LoginAttempts]++;
	        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Xyronite", str, "Login", "Exit");
			return 1;
		}
		else
		{
		    //SendServerMessage(playerid, "Kamu telah salah memasukan password sebanyak {FFFF00}5 kali!");
		    DelayedKick(playerid);
			return 1;
		}
	}
	cache_set_active(UcpData[playerid][Cache_ID]);

	// remove the active cache from memory and unsets the active cache as well
	
	new query[256];
	format(query, sizeof(query), "SELECT `name` FROM `characters` WHERE `UCP` = '%s' LIMIT %d;", UcpData[playerid][UcpName], MAX_CHARS);
	mysql_tquery(g_SQL, query, "LoadCharacter", "d", playerid);
	cache_delete(UcpData[playerid][Cache_ID]);
	UcpData[playerid][Cache_ID] = MYSQL_INVALID_CACHE;

	KillTimer(UcpData[playerid][LoginTimer]);
	UcpData[playerid][LoginTimer] = 0;
	UcpData[playerid][IsLoggedIn] = true;
	return 1;
}

FUNC::HashPlayerPassword(playerid, hashid)
{
	new
		query[256],
		hash[BCRYPT_HASH_LENGTH];

    bcrypt_get_hash(hash, sizeof(hash));

	GetPlayerName(playerid, UcpData[playerid][UcpName], MAX_PLAYER_NAME + 1);

	mysql_format(g_SQL, query, sizeof query, "INSERT INTO `players` (`username`, `password`) VALUES ('%e', '%s')", UcpData[playerid][UcpName], hash);
	mysql_tquery(g_SQL, query, "OnPlayerRegister", "d", playerid);

    //SendServerMessage(playerid, "Your UCP is successfully registered!");
    //CheckAccount(playerid);
	return 1;
}

FUNC::LoadCharacter(playerid)
{
	for (new i = 0; i < MAX_CHARS; i ++)
	{
		PlayerChar[playerid][i][0] = EOS;
	}
	for (new i = 0; i < cache_num_rows(); i ++)
	{
		cache_get_value_name(i, "name", PlayerChar[playerid][i]);
	}
  	ShowCharacterList(playerid);
  	return 1;
}

ShowCharacterList(playerid)
{
	new name[256], count, sgstr[128];

	for (new i; i < MAX_CHARS; i ++) if(PlayerChar[playerid][i][0] != EOS)
	{
	    format(sgstr, sizeof(sgstr), "%s\n", PlayerChar[playerid][i]);
		strcat(name, sgstr);
		count++;
	}
	if(count < MAX_CHARS)
		strcat(name, "< Create Character >");

	ShowPlayerDialog(playerid, DIALOG_CHARLIST, DIALOG_STYLE_LIST, "Character List", name, "Select", "Quit");
	return 1;
}






stock GetName(playerid)
{
	new name[MAX_PLAYER_NAME];
 	GetPlayerName(playerid,name,sizeof(name));
	return name;
}

FUNC::InsertPlayerName(playerid, name[])
{
	new count = cache_num_rows(), query[145], Cache:execute;
	if(count > 0)
	{
        ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "ERROR: This name is already used by the other player!\nInsert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");
	}
	else
	{
		mysql_format(g_SQL,query,sizeof(query),"INSERT INTO `characters` (`name`,`UCP`) VALUES('%e','%e')",name,GetName(playerid));
		execute = mysql_query(g_SQL, query);
		pInfo[playerid][pID] = cache_insert_id();
	 	cache_delete(execute);
	 	SetPlayerName(playerid, name);
		format(pInfo[playerid][pName], MAX_PLAYER_NAME, name);
		ResetCharacterVariable(playerid);
		pInfo[playerid][pPosX] = DEFAULT_POS_X;
		pInfo[playerid][pPosY] = DEFAULT_POS_Y;
		pInfo[playerid][pPosZ] = DEFAULT_POS_Z;
		pInfo[playerid][pPosAngle] = DEFAULT_POS_A;
		SetSpawnInfo(playerid, NO_TEAM, pInfo[playerid][pSkin], pInfo[playerid][pPosX], pInfo[playerid][pPosY], pInfo[playerid][pPosZ], pInfo[playerid][pPosAngle]);
		SpawnPlayer(playerid);
	 	//ShowPlayerDialog(playerid, DIALOG_AGE, DIALOG_STYLE_INPUT, "Character Age", "Please Insert your Character Age", "Continue", "Cancel");
	}
	return 1;
}