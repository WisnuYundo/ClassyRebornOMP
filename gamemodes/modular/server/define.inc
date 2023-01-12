#define forex(%0,%1) for(new %0 = 0; %0 < %1; %0++)

#define FUNC::%0(%1) forward %0(%1); public %0(%1)

#define COLOR_YELLOW 			0xFFFF00FF
#define COLOR_SERVER 			0x00FFFFFF
#define COLOR_GREY   			0xAFAFAFFF
#define COLOR_PURPLE 			0xD0AEEBFF
#define COLOR_CLIENT 			0xC6E2FFFF
#define COLOR_WHITE  			0xFFFFFFFF
#define COLOR_LIGHTRED    		0xFF6347FF

#define DEFAULT_POS_X 1644.8433
#define DEFAULT_POS_Y -2286.5952
#define DEFAULT_POS_Z -1.2025
#define DEFAULT_POS_A 269.9897

#define MAX_CHARS 3

#define		SECONDS_TO_LOGIN 	30

#if !defined BCRYPT_HASH_LENGTH
	#define BCRYPT_HASH_LENGTH 250
#endif

#if !defined BCRYPT_COST
	#define BCRYPT_COST 12
#endif