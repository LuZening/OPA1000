/*
 * FS.c
 *
 *  Created on: Sep 12, 2019
 *      Author: Zening
 */
#include <string.h>
#include "FS.h"
FS_typedef FS;

BOOL FS_begin(FS_typedef* pFS, uint32_t* addr_base)
{
	if((uint32_t)addr_base < MAX_ADDR && ((pFS->n_files = *(uint32_t*)(addr_base)) != 0xffffffff))
	{
		pFS->addr_base = addr_base;
		return true;
	}
	else
	{
		return false;
	}
}

FSfile_typedef FS_open(FS_typedef* pFS, const char* path)
{
	int i = pFS->n_files;
	uint32_t* addr = (uint32_t*)pFS->addr_base;  // base_addr: number_of_files:4B, addr_file_nodes:4B[n_files]
	uint32_t offset = (uint32_t)pFS->addr_base;

	FSfile_typedef file;
	file.size = 0;
	file.p_content = NULL;
	file.path = NULL;
	do{
		addr ++;
		i--;
		char* _path = (char*)((uint32_t*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0) // path matched
		{
			file.path = _path;
			// 4-Byte alignment!!!!!!!!!
			file.p_content = (uint8_t*)(*((uint32_t*)(*addr + offset) + 1)+offset);
			file.size = *(uint32_t*)(*addr + offset);
			break;
		}
	}while(i > 0);
	return file;
}

int FS_size(FS_typedef* pFS, const char* path)
{
	uint32_t i;
	uint32_t* addr = (uint32_t*)pFS->addr_base;
	uint32_t offset = (uint32_t)pFS->addr_base;
	for(i=0; i<pFS->n_files; ++i)
	{
		addr++;
		char* _path = (char*)((uint32_t*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0)
		{
			return *(uint32_t*)(*addr + offset);
		}
	}
	return -1;
}

BOOL FS_exists(FS_typedef* pFS, const char* path)
{
	uint32_t i;
	uint32_t* addr = (uint32_t*)pFS->addr_base;
	uint32_t offset = (uint32_t)pFS->addr_base;
	for(i=0; i<pFS->n_files; ++i)
	{
		addr++;
		char* _path = (char*)((uint32_t*)(*addr+offset)+2);
		if(strcmp(path, _path) == 0)
		{
			return true;
		}
	}
	return false;
}
