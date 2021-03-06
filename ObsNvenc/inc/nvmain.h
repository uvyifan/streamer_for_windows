/********************************************************************************
 Copyright (C) 2014 Timo Rothenpieler <timo@rothenpieler.org>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/

#ifndef H_OBSNVENC_MAIN__H
#define H_OBSNVENC_MAIN__H

#pragma warning(disable: 4005)

#include <Main.h>
#include "cudaDynload.h"
#include "nvEncodeAPI.h"

extern ConfigFile **NvAppConfig;
extern bool doLog;
#define NvLog(...) if(doLog) Log(__VA_ARGS__)
#define AppConfig (*NvAppConfig)

typedef NVENCSTATUS(NVENCAPI* PNVENCODEAPICREATEINSTANCE)(NV_ENCODE_API_FUNCTION_LIST *functionList);
extern NV_ENCODE_API_FUNCTION_LIST *pNvEnc;

extern int iNvencDeviceCount;
extern CUdevice pNvencDevices[16];
extern unsigned int iNvencUseDeviceID;

bool _checkCudaErrors(CUresult err, const char *func);
#define checkCudaErrors(f) if(!_checkCudaErrors(f, #f)) goto error

bool checkNvEnc();
bool initNvEnc();
void deinitNvEnc();

extern unsigned int encoderRefs;
void encoderRefDec();

struct OSMutexLocker
{
    HANDLE& h;
    bool enabled;

    OSMutexLocker(HANDLE &h) : h(h), enabled(true) { OSEnterMutex(h); }
    ~OSMutexLocker() { if (enabled) OSLeaveMutex(h); }
    OSMutexLocker(OSMutexLocker &&other) : h(other.h), enabled(other.enabled) { other.enabled = false; }
};

template<typename T>
inline bool dataEqual(const T& a, const T& b)
{
    return memcmp(&a, &b, sizeof(T)) == 0;
}

String guidToString(const GUID &guid);
bool stringToGuid(const String &string, GUID *guid);

#endif
