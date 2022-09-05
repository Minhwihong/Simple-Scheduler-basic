#ifndef __MODEL_CONFIG_H__
#define __MODEL_CONFIG_H__

#include <stdint.h>
#include "printf.h"
//#include <stdio.h>


#define MODULE_TEST		0



#define PRINT_CAN(...)	printf(__VA_ARGS__)
#define PRINT_DBG(...) 	printf(__VA_ARGS__)
#define PRINT_ESP(...) 	printf(__VA_ARGS__)




/* ********** Simple Primitive Type Redefinition ********** */

#define u8	uint8_t
#define u16	uint16_t
#define u32	uint32_t

#define s8	int8_t
#define s16	int16_t
#define s32	int32_t

#define BOOL	u8
#define TRUE	1
#define FALSE	0
/* ******************************************************** */


#define NAME_LEN    28



typedef struct tag_version{
	uint8_t ma;
	uint8_t mi;
	uint8_t rev;
	uint8_t res;

}version_t;



typedef struct tag_myInfo{
	version_t sw_ver;
	version_t comm_ver;
	char	name[NAME_LEN];
	uint8_t	verifyKey[12];
	u32	CANId;

}myInfo_t;




#if(IH_USE == 1)
#define GLB_OPT_DEFAULT_IH			1
#else
#define GLB_OPT_DEFAULT_IH			0
#endif








#define DEFAULT_CAN_ID			(u32)0x310


#define DISP_LABEL 		"Application"



#define VERSION_MA	0
#define VERSION_MI	1
#define VERSION_REV	2
#define VERSION_RES	3

#define COMM_VER_MA		0
#define COMM_VER_MI		2
#define COMM_VER_REV   	5	




#define PROD_NAME "my Application\0"
#define VERIFY_CODE {0,1,2,3,4,5,0,1,2,3,4,5}


#endif


