/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../../config.h"
#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_string.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_urlmon_hooks.h"

int32_t	env_w32_hook_URLDownloadToFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
HRESULT URLDownloadToFile(
  LPUNKNOWN pCaller,
  LPCTSTR szURL,
  LPCTSTR szFileName,
  DWORD dwReserved,
  LPBINDSTATUSCALLBACK lpfnCB
);
*/
	emu_profile_function_add(env->profile, "URLDownloadToFile");

	uint32_t p_caller;
	POP_DWORD(c, &p_caller);
	emu_profile_argument_add_ptr(env->profile, "LPUNKNOWN", "pCaller", p_caller);
	emu_profile_argument_add_none(env->profile);

	uint32_t p_url;
	POP_DWORD(c, &p_url);
	emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "szURL", p_url);

    struct emu_string *url = emu_string_new();
	emu_memory_read_string(c->mem, p_url, url, 512);
	emu_profile_argument_add_string(env->profile, "", "", emu_string_char(url)); 


	uint32_t p_filename;
	POP_DWORD(c, &p_filename);
	emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "szFileName", p_filename);

	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(c->mem, p_filename, filename, 512);
	emu_profile_argument_add_string(env->profile, "", "", emu_string_char(filename)); 

	uint32_t reserved;
	POP_DWORD(c, &reserved);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwReserved", reserved);

	uint32_t statuscallbackfn;
	POP_DWORD(c, &statuscallbackfn);
	emu_profile_argument_add_int(env->profile, "LPBINDSTATUSCALLBACK", "lpfnCB", statuscallbackfn);






//	printf(" %s -> %s\n", emu_string_char(url), emu_string_char(filename));

	emu_string_free(url);
	emu_string_free(filename);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

