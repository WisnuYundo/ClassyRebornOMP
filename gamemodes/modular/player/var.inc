//enum
new PlayerChar[MAX_PLAYERS][MAX_CHARS][MAX_PLAYER_NAME + 1];

enum E_UCP
{
	UcpID,
	UcpName[MAX_PLAYER_NAME],
	UcpPassword[65], // the output of SHA256_PassHash function (which was added in 0.3.7 R1 version) is always 256 bytes in length, or the equivalent of 64 Pawn cells

	Cache: Cache_ID,
	bool: IsLoggedIn,
	LoginAttempts,
	LoginTimer
};
new UcpData[MAX_PLAYERS][E_UCP];

enum E_PLAYERS
{
	pID,
	pName[MAX_PLAYER_NAME],
	pUCP[MAX_PLAYER_NAME],
	pPassword,
	pSkin,
	pInterior,
	pWorld,
	Float:pPosX,
	Float:pPosY,
	Float:pPosZ,
	Float:pPosAngle,
	bool:pSpawned,
	pMoney,
	Float:pHealth,
	Float:pArmour,
	pLevel,
	pExp
};
new pInfo[MAX_PLAYERS][E_PLAYERS];