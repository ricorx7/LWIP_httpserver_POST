/*
 * fsdata_custom.c
 *
 * Created: 2/9/2018 9:23:18 AM
 *  Author: rico
 */ 


#include "lwip/opt.h"
#include "lwip/def.h"
#include "fs.h"
#include "fsdata.h"
#include "ff.h"
#include "rti/rti_sdA.h"
#include <string.h>

/*-----------------------------------------------------------------------------------*/
int fs_open_custom(struct fs_file *file, const char *name)
{
	FIL file_object;

	int result = rtisdA_Init(0);
	if( result != FR_OK )
	{
		return SD_ERROR_MOUNT;
	}
	result = f_open(&file_object, name,	FA_READ);
	if( result != FR_OK )
	{
		f_close(&file_object);
		return SD_ERROR_OPEN;
	}
	
	return file_object.fsize;;
}

/*-----------------------------------------------------------------------------------*/
void fs_close_custom(struct fs_file *file)
{
	
	
}