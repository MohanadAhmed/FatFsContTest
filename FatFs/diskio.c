/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <stdio.h>
#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

static FILE* file;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	//printf(__FUNCTION__"\n");
	file = fopen("test.bin", "r+b");
	printf(__FUNCTION__);
	if (!file){
		printf("disk init error\n");
	}else{
		return 0;
	}
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
extern int _lastSectorNum;
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	//printf(__FUNCTION__"\n");
	_lastSectorNum = sector;
	if(!file){
		printf("Error\n");
		return 1;
	}else{
		int n;
		if(fseek(file, sector * 512, SEEK_SET) == 0){
			//printf("seek ok\n");
			int r;
			r = fread(buff, 1, count*512, file);
			//printf("\t sector = %d, r = %d, count = %d\n", sector, r/512, count);
			if( (r/512) == count) return 0; else return 1;
		}else{
			return 1;
		}
	}
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	//printf(__FUNCTION__"\n");
	_lastSectorNum = sector;
	if(!file){
		printf("Error");
	}else{
		int n;
		fseek(file, sector * 512, SEEK_SET);
		int r = fwrite(buff, 1, count*512, file);
		//printf("\t sector = %d, r = %d, count = %d\n", sector, r/512, count);
		if( (r/512) == count) return 0; else return 1;
	}
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	//printf(__FUNCTION__"\n");
	switch(cmd){
		case CTRL_SYNC:
			fflush(file);
			break;
		case GET_SECTOR_COUNT:
			*((unsigned long*)buff) = (2*1024*1024*2);
			break;
		case GET_SECTOR_SIZE:
			*((unsigned long*)buff) = (512);
			break;
		case GET_BLOCK_SIZE:
			*((unsigned long*)buff) = (512);
			break;
		case CTRL_TRIM:
			
			break;
	}
	return RES_OK;
}

