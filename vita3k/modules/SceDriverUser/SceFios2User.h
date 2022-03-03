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

#pragma once

#include <module/module.h>

typedef int32_t SceFiosOverlayID;

struct SceFiosOverlay {
    uint8_t type;
    uint8_t order;
    uint16_t dst_len;
    uint16_t src_len;
    uint16_t unk2;
    SceUID pid;
    SceFiosOverlayID id;
    char dst[292];
    char src[292]; // src path replaces dst path based on type policy
};

struct sceFiosOverlayGetRecommendedScheduler_opt {
    SceSize pOutPath;
    SceSize maxPath;
    SceSize loOrderFilter;
    SceSize hiOrderFilter;
    SceSize reserved1;
    SceSize reserved2;
    SceSize reserved3;
    SceSize reserved4;
    SceSize reserved5;
    SceSize reserved6;
};

BRIDGE_DECL(sceFiosOverlayAddForProcess02)
BRIDGE_DECL(sceFiosOverlayGetInfoForProcess02)
BRIDGE_DECL(sceFiosOverlayGetList02)
BRIDGE_DECL(sceFiosOverlayGetRecommendedScheduler02)
BRIDGE_DECL(sceFiosOverlayModifyForProcess02)
BRIDGE_DECL(sceFiosOverlayRemoveForProcess02)
BRIDGE_DECL(sceFiosOverlayResolveSync02)
BRIDGE_DECL(sceFiosOverlayResolveWithRangeSync02)
BRIDGE_DECL(sceFiosOverlayThreadIsDisabled02)
BRIDGE_DECL(sceFiosOverlayThreadSetDisabled02)
