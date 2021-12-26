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

#include "SceFios2User.h"

EXPORT(int, sceFiosOverlayAddForProcess02, SceUID pid, SceFiosOverlay *overlay, SceFiosOverlayID *outID) {
    LOG_DEBUG("pid: {}", pid);
    LOG_DEBUG("dst: {}", overlay->dst);
    LOG_DEBUG("dst len: {}", overlay->dst_len);
    LOG_DEBUG("id: {}", overlay->id);
    LOG_DEBUG("order: {}", overlay->order);
    LOG_DEBUG("pid: {}", overlay->pid);
    LOG_DEBUG("src: {}", overlay->src);
    LOG_DEBUG("src_len: {}", overlay->src_len);
    LOG_DEBUG("type: {}", overlay->type);
    LOG_DEBUG("outID: {}", *outID);
    return 0;
}

EXPORT(int, sceFiosOverlayGetInfoForProcess02) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayGetList02, SceFiosOverlayID *pOutIDs, size_t maxIDs, size_t *pActualIDs) {
    LOG_DEBUG("maxIDs: {}", maxIDs);
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayGetRecommendedScheduler02, SceUID pid, const char *path, const char *pInPath, char *pOutPath) {
    //LOG_DEBUG("pid: {}, path: {}, pInPath: {}", pid, path, pInPath);
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayModifyForProcess02) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayRemoveForProcess02) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayResolveSync02) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayResolveWithRangeSync02, SceUID pid, int resolveFlag, const char *pInPath, char *pOutPath, size_t maxPath) {
    STUBBED("Using strncpy");
    strncpy(pOutPath, pInPath, maxPath);

    return 0;
}

EXPORT(int, sceFiosOverlayThreadIsDisabled02) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceFiosOverlayThreadSetDisabled02) {
    return UNIMPLEMENTED();
}

BRIDGE_IMPL(sceFiosOverlayAddForProcess02)
BRIDGE_IMPL(sceFiosOverlayGetInfoForProcess02)
BRIDGE_IMPL(sceFiosOverlayGetList02)
BRIDGE_IMPL(sceFiosOverlayGetRecommendedScheduler02)
BRIDGE_IMPL(sceFiosOverlayModifyForProcess02)
BRIDGE_IMPL(sceFiosOverlayRemoveForProcess02)
BRIDGE_IMPL(sceFiosOverlayResolveSync02)
BRIDGE_IMPL(sceFiosOverlayResolveWithRangeSync02)
BRIDGE_IMPL(sceFiosOverlayThreadIsDisabled02)
BRIDGE_IMPL(sceFiosOverlayThreadSetDisabled02)
