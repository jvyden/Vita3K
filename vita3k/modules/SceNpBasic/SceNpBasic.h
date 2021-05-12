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

enum SceNpBasicErrorCode {
    SCE_NP_BASIC_ERROR_BASE = 0x80551d00,
    SCE_NP_BASIC_ERROR_UNKNOWN = 0x80551d01,
    SCE_NP_BASIC_ERROR_INVALID_ARGUMENT = 0x80551d02,
    SCE_NP_BASIC_ERROR_NOT_INITIALIZED = 0x80551d04,
    SCE_NP_BASIC_ERROR_ALREADY_INITIALIZED = 0x80551d05,
    SCE_NP_BASIC_ERROR_SIGNED_OUT = 0x80551d06,
    SCE_NP_BASIC_ERROR_NOT_ONLINE = 0x80551d07,
    SCE_NP_BASIC_ERROR_DATA_NOT_FOUND = 0x80551d08,
    SCE_NP_BASIC_ERROR_BUSY = 0x80551d09,
    SCE_NP_BASIC_ERROR_NOT_READY_TO_COMMUNICATE = 0x80551d0a,
    SCE_NP_BASIC_ERROR_NO_COMM_ID_SUPPLIED = 0x80551d0b,
    SCE_NP_BASIC_ERROR_IPC_QUEUE_OVERFLOWED = 0x80551d0c,
    SCE_NP_BASIC_ERROR_NO_NP_COMM_ID_IN_PARAM_SFO = 0x80551d0e,
    SCE_NP_BASIC_ERROR_JOIN_GAME_ACK_REQUEST_TIMEDOUT = 0x80551d0f,
    SCE_NP_BASIC_ERROR_JOIN_GAME_ACK_REQUEST_CANCELED = 0x80551d10,
    SCE_NP_BASIC_ERROR_JOIN_GAME_ACK_REQUEST_INVALID = 0x80551d11,
    SCE_NP_BASIC_ERROR_NO_JOIN_GAME_ACK_HANDLER = 0x80551d12,
};

BRIDGE_DECL(sceNpBasicCheckCallback)
BRIDGE_DECL(sceNpBasicCheckIfPlayerIsBlocked)
BRIDGE_DECL(sceNpBasicGetBlockListEntries)
BRIDGE_DECL(sceNpBasicGetBlockListEntryCount)
BRIDGE_DECL(sceNpBasicGetFriendContextState)
BRIDGE_DECL(sceNpBasicGetFriendListEntries)
BRIDGE_DECL(sceNpBasicGetFriendListEntryCount)
BRIDGE_DECL(sceNpBasicGetFriendOnlineStatus)
BRIDGE_DECL(sceNpBasicGetFriendRequestEntries)
BRIDGE_DECL(sceNpBasicGetFriendRequestEntryCount)
BRIDGE_DECL(sceNpBasicGetGameJoiningPresence)
BRIDGE_DECL(sceNpBasicGetGamePresenceOfFriend)
BRIDGE_DECL(sceNpBasicGetPlaySessionLog)
BRIDGE_DECL(sceNpBasicGetPlaySessionLogSize)
BRIDGE_DECL(sceNpBasicGetRequestedFriendRequestEntries)
BRIDGE_DECL(sceNpBasicGetRequestedFriendRequestEntryCount)
BRIDGE_DECL(sceNpBasicInit)
BRIDGE_DECL(sceNpBasicJoinGameAckResponseSend)
BRIDGE_DECL(sceNpBasicRecordPlaySessionLog)
BRIDGE_DECL(sceNpBasicRegisterHandler)
BRIDGE_DECL(sceNpBasicRegisterInGameDataMessageHandler)
BRIDGE_DECL(sceNpBasicRegisterJoinGameAckHandler)
BRIDGE_DECL(sceNpBasicSendInGameDataMessage)
BRIDGE_DECL(sceNpBasicSetInGamePresence)
BRIDGE_DECL(sceNpBasicTerm)
BRIDGE_DECL(sceNpBasicUnregisterHandler)
BRIDGE_DECL(sceNpBasicUnregisterInGameDataMessageHandler)
BRIDGE_DECL(sceNpBasicUnregisterJoinGameAckHandler)
BRIDGE_DECL(sceNpBasicUnsetInGamePresence)
