// 
//   Copyright (C) 2005, 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

////////////////////////////////////////////////////////////
//
// Implementation of Netscape entry points (NPN_*)
//

#include "pluginbase.h"

extern NPNetscapeFuncs NPNFuncs;

void NPN_Version(int* plugin_major, int* plugin_minor, int* netscape_major, int* netscape_minor)
{
  *plugin_major   = NP_VERSION_MAJOR;
  *plugin_minor   = NP_VERSION_MINOR;
  *netscape_major = HIBYTE(NPNFuncs.version);
  *netscape_minor = LOBYTE(NPNFuncs.version);
}

NPError NPN_GetURLNotify(NPP instance, const char *url, const char *target, void* notifyData)
{
	int navMinorVers = NPNFuncs.version & 0xFF;
    NPError rv = NPERR_NO_ERROR;

    if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
        rv = CallNPN_GetURLNotifyProc(NPNFuncs.geturlnotify, instance, url, target, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;
    
    return rv;
}

NPError NPN_GetURL(NPP instance, const char *url, const char *target)
{
    NPError rv = CallNPN_GetURLProc(NPNFuncs.geturl, instance, url, target);
    return rv;
}

NPError NPN_PostURLNotify(NPP instance, const char* url, const char* window, uint32 len, const char* buf, NPBool file, void* notifyData)
{
	int navMinorVers = NPNFuncs.version & 0xFF;
  NPError rv = NPERR_NO_ERROR;

	if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
		rv = CallNPN_PostURLNotifyProc(NPNFuncs.posturlnotify, instance, url, window, len, buf, file, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

NPError NPN_PostURL(NPP instance, const char* url, const char* window, uint32 len, const char* buf, NPBool file)
{
  NPError rv = CallNPN_PostURLProc(NPNFuncs.posturl, instance, url, window, len, buf, file);
  return rv;
} 

NPError NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
  NPError rv = CallNPN_RequestReadProc(NPNFuncs.requestread, stream, rangeList);
  return rv;
}

NPError NPN_NewStream(NPP instance, NPMIMEType type, const char* target, NPStream** stream)
{
	int navMinorVersion = NPNFuncs.version & 0xFF;

  NPError rv = NPERR_NO_ERROR;

	if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_NewStreamProc(NPNFuncs.newstream, instance, type, target, stream);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

int32 NPN_Write(NPP instance, NPStream *stream, int32 len, void *buffer)
{
	int navMinorVersion = NPNFuncs.version & 0xFF;
  int32 rv = 0;

  if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_WriteProc(NPNFuncs.write, instance, stream, len, buffer);
	else
		rv = -1;

  return rv;
}

NPError NPN_DestroyStream(NPP instance, NPStream* stream, NPError reason)
{
	int navMinorVersion = NPNFuncs.version & 0xFF;
  NPError rv = NPERR_NO_ERROR;

  if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_DestroyStreamProc(NPNFuncs.destroystream, instance, stream, reason);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

  return rv;
}

void NPN_Status(NPP instance, const char *message)
{
  CallNPN_StatusProc(NPNFuncs.status, instance, message);
}

const char* NPN_UserAgent(NPP instance)
{
  const char * rv = NULL;
  rv = CallNPN_UserAgentProc(NPNFuncs.uagent, instance);
  return rv;
}

void* NPN_MemAlloc(uint32 size)
{
  void * rv = NULL;
  rv = CallNPN_MemAllocProc(NPNFuncs.memalloc, size);
  return rv;
}

void NPN_MemFree(void* ptr)
{
  CallNPN_MemFreeProc(NPNFuncs.memfree, ptr);
}

uint32 NPN_MemFlush(uint32 size)
{
  uint32 rv = CallNPN_MemFlushProc(NPNFuncs.memflush, size);
  return rv;
}

void NPN_ReloadPlugins(NPBool reloadPages)
{
  CallNPN_ReloadPluginsProc(NPNFuncs.reloadplugins, reloadPages);
}

NPError NPN_GetValue(NPP instance, NPNVariable variable, void *value)
{
  NPError rv = CallNPN_GetValueProc(NPNFuncs.getvalue, instance, variable, value);
  return rv;
}

NPError NPN_SetValue(NPP instance, NPPVariable variable, void *value)
{
  NPError rv = CallNPN_SetValueProc(NPNFuncs.setvalue, instance, variable, value);
  return rv;
}

void NPN_InvalidateRect(NPP instance, NPRect *invalidRect)
{
  CallNPN_InvalidateRectProc(NPNFuncs.invalidaterect, instance, invalidRect);
}

void NPN_InvalidateRegion(NPP instance, NPRegion invalidRegion)
{
  CallNPN_InvalidateRegionProc(NPNFuncs.invalidateregion, instance, invalidRegion);
}

void NPN_ForceRedraw(NPP instance)
{
  CallNPN_ForceRedrawProc(NPNFuncs.forceredraw, instance);
}

// The following has been copied from a newer version of npn_gate.cpp

NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name)
{
  return NPNFuncs.getstringidentifier(name);
}

bool NPN_GetProperty(NPP npp, NPObject* obj, NPIdentifier propertyName,
                     NPVariant *result)
{
  return NPNFuncs.getproperty(npp, obj, propertyName, result);
}

void NPN_ReleaseObject(NPObject *obj)
{
  return NPNFuncs.releaseobject(obj);
}

void NPN_PluginThreadAsyncCall (NPP instance, void (*func)(void *), void  *userData)
{
	CallNPN_PluginThreadAsyncCallProc(NPNFuncs.pluginthreadasynccall,
		instance, func,	userData);
}
