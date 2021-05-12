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

#include <cstdint>
#include <cstring>
#include <mem/ptr.h>

namespace np {

enum SceNpCountryCode {
    SCE_NP_LANG_JAPANESE = 0,
    SCE_NP_LANG_ENGLISH = 1,
    SCE_NP_LANG_ENGLISH_US = 1,
    SCE_NP_LANG_FRENCH = 2,
    SCE_NP_LANG_SPANISH = 3,
    SCE_NP_LANG_GERMAN = 4,
    SCE_NP_LANG_ITALIAN = 5,
    SCE_NP_LANG_DUTCH = 6,
    SCE_NP_LANG_PORTUGUESE = 7,
    SCE_NP_LANG_PORTUGUESE_PT = 7,
    SCE_NP_LANG_RUSSIAN = 8,
    SCE_NP_LANG_KOREAN = 9,
    SCE_NP_LANG_CHINESE_T = 10,
    SCE_NP_LANG_CHINESE_S = 11,
    SCE_NP_LANG_FINNISH = 12,
    SCE_NP_LANG_SWEDISH = 13,
    SCE_NP_LANG_DANISH = 14,
    SCE_NP_LANG_NORWEGIAN = 15,
    SCE_NP_LANG_POLISH = 16,
    SCE_NP_LANG_PORTUGUESE_BR = 17,
    SCE_NP_LANG_ENGLISH_GB = 18,
    SCE_NP_LANG_TURKISH = 19,
    SCE_NP_LANG_SPANISH_LA = 20,
    SCE_NP_LANG_ARABIC = 21,
    SCE_NP_LANG_FRENCH_CA
};

struct NpOnlineID {
    // Maximum name is 16 bytes
    char name[16];
    char term;
    char dummy[3];
};

struct NpId {
    NpOnlineID online_id;
    std::uint8_t opt[8];
    std::uint8_t unk0[8];
};

struct CommunicationID {
    char data[9];
    char term;
    std::uint8_t num;
    char dummy;
};

inline const bool operator==(const CommunicationID &lhs, const CommunicationID &rhs) {
    return (strncmp(lhs.data, rhs.data, 9) == 0) && (lhs.num == rhs.num);
}

struct CommunicationConfig {
    Ptr<CommunicationID> comm_id;
    Ptr<void> unk0;
    Ptr<void> unk1;
};

enum class NpTrophyError {
    TROPHY_ERROR_NONE = 0,
    TROPHY_CONTEXT_EXIST = 1,
    TROPHY_CONTEXT_FILE_NON_EXIST = 2,
    TROPHY_ID_INVALID = 3,
    TROPHY_ALREADY_UNLOCKED = 4,
    TROPHY_PLATINUM_IS_UNBREAKABLE = 5, // Platinum is unbreakable
};

} // namespace np
