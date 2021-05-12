// Vita3K emulator project
// Copyright (C) 2021 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "SceDisplay.h"

#include <display/functions.h>
#include <host/functions.h>
#include <util/types.h>

static bool use_multi;
static int display_wait(HostState &host, SceUID current_thread, const std::int32_t vcount, bool cb) {
    if (cb)
        process_callbacks(host.kernel, current_thread);

    wait_vblank(host.display, host.kernel, current_thread, vcount);

    if (host.display.abort.load())
        return SCE_DISPLAY_ERROR_NO_PIXEL_DATA;

    return SCE_DISPLAY_ERROR_OK;
}

EXPORT(int, _sceDisplayGetFrameBuf) {
    return UNIMPLEMENTED();
}

EXPORT(int, _sceDisplayGetFrameBufInternal) {
    return UNIMPLEMENTED();
}

EXPORT(int, _sceDisplayGetMaximumFrameBufResolution) {
    return UNIMPLEMENTED();
}

EXPORT(int, _sceDisplayGetResolutionInfoInternal) {
    return UNIMPLEMENTED();
}

EXPORT(SceInt32, _sceDisplaySetFrameBuf, const SceDisplayFrameBuf *pFrameBuf, SceDisplaySetBufSync sync) {
    if (!pFrameBuf)
        return SCE_DISPLAY_ERROR_OK;
    if (pFrameBuf->size != sizeof(SceDisplayFrameBuf)) {
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_VALUE);
    }
    if (!pFrameBuf->base) {
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_ADDR);
    }
    if (pFrameBuf->pitch < pFrameBuf->width) {
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_PITCH);
    }
    if (pFrameBuf->pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_PIXELFORMAT);
    }
    if (sync != SCE_DISPLAY_SETBUF_NEXTFRAME && sync != SCE_DISPLAY_SETBUF_IMMEDIATE) {
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_UPDATETIMING);
    }
    if ((pFrameBuf->width < 480) || (pFrameBuf->height < 272) || (pFrameBuf->pitch < 480))
        return RET_ERROR(SCE_DISPLAY_ERROR_INVALID_RESOLUTION);

    host.display.base = pFrameBuf->base;
    host.display.pitch = pFrameBuf->pitch;
    host.display.pixelformat = pFrameBuf->pixelformat;
    host.display.image_size.x = pFrameBuf->width;
    host.display.image_size.y = pFrameBuf->height;
    host.frame_count++;

    MicroProfileFlip(nullptr);

    return SCE_DISPLAY_ERROR_OK;
}

EXPORT(int, _sceDisplaySetFrameBufForCompat) {
    return UNIMPLEMENTED();
}

EXPORT(int, _sceDisplaySetFrameBufInternal) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceDisplayGetPrimaryHead) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceDisplayGetRefreshRate, float *pFps) {
    *pFps = 59.94005f;
    return 0;
}

EXPORT(int, sceDisplayGetVcount) {
    return static_cast<int>(host.display.vblank_count.load());
}

EXPORT(int, sceDisplayGetVcountInternal) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceDisplayRegisterVblankStartCallback) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceDisplayUnregisterVblankStartCallback) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceDisplayWaitSetFrameBuf) {
    STUBBED("move after setframebuf");
    //LOG_DEBUG("WaitSetFrameBuf: 1");
    return use_multi ? 0 : display_wait(host, thread_id, static_cast<std::int32_t>(1), false);
}

EXPORT(int, sceDisplayWaitSetFrameBufCB) {
    STUBBED("move after setframebuf");
    //LOG_DEBUG("WaitSetFrameBufCB: 1");
    return use_multi ? 0 : display_wait(host, thread_id, 1, true);
}

EXPORT(int, sceDisplayWaitSetFrameBufMulti, SceUInt vcount) {
    STUBBED("move after setframebuf");
    //LOG_DEBUG("WaitSetFrameBufMulti: {}", vcount);
    use_multi = true;
    return display_wait(host, thread_id, static_cast<std::int32_t>(vcount), false);
}

EXPORT(int, sceDisplayWaitSetFrameBufMultiCB, SceUInt vcount) {
    STUBBED("move after setframebuf");
    //LOG_DEBUG("WaitSetFrameBufMultiCB: {}", vcount);
    use_multi = true;
    return display_wait(host, thread_id, static_cast<std::int32_t>(vcount), true);
}

EXPORT(int, sceDisplayWaitVblankStart) {
    STUBBED("wait for vblank");
    //LOG_DEBUG("WaitVblankStart: 1");
    return use_multi ? 0 : display_wait(host, thread_id, 1, false);
}

EXPORT(int, sceDisplayWaitVblankStartCB) {
    STUBBED("wait for vblank");
    //LOG_DEBUG("WaitVblankStartCB: 1");
    return use_multi ? 0 : display_wait(host, thread_id, 1, true);
}

EXPORT(int, sceDisplayWaitVblankStartMulti, SceUInt vcount) {
    STUBBED("wait for vblank");
    //LOG_DEBUG("WaitVblankStartMulti: {}", vcount);
    use_multi = true;
    return display_wait(host, thread_id, static_cast<std::int32_t>(vcount), false);
}

EXPORT(int, sceDisplayWaitVblankStartMultiCB, SceUInt vcount) {
    STUBBED("wait for vblank");
    //LOG_DEBUG("WaitVblankStartMultiCB: {}", vcount);
    use_multi = true;
    return display_wait(host, thread_id, static_cast<std::int32_t>(vcount), false);
}

BRIDGE_IMPL(_sceDisplayGetFrameBuf)
BRIDGE_IMPL(_sceDisplayGetFrameBufInternal)
BRIDGE_IMPL(_sceDisplayGetMaximumFrameBufResolution)
BRIDGE_IMPL(_sceDisplayGetResolutionInfoInternal)
BRIDGE_IMPL(_sceDisplaySetFrameBuf)
BRIDGE_IMPL(_sceDisplaySetFrameBufForCompat)
BRIDGE_IMPL(_sceDisplaySetFrameBufInternal)
BRIDGE_IMPL(sceDisplayGetPrimaryHead)
BRIDGE_IMPL(sceDisplayGetRefreshRate)
BRIDGE_IMPL(sceDisplayGetVcount)
BRIDGE_IMPL(sceDisplayGetVcountInternal)
BRIDGE_IMPL(sceDisplayRegisterVblankStartCallback)
BRIDGE_IMPL(sceDisplayUnregisterVblankStartCallback)
BRIDGE_IMPL(sceDisplayWaitSetFrameBuf)
BRIDGE_IMPL(sceDisplayWaitSetFrameBufCB)
BRIDGE_IMPL(sceDisplayWaitSetFrameBufMulti)
BRIDGE_IMPL(sceDisplayWaitSetFrameBufMultiCB)
BRIDGE_IMPL(sceDisplayWaitVblankStart)
BRIDGE_IMPL(sceDisplayWaitVblankStartCB)
BRIDGE_IMPL(sceDisplayWaitVblankStartMulti)
BRIDGE_IMPL(sceDisplayWaitVblankStartMultiCB)
