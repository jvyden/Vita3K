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

#include <util/types.h>

#include <SDL_audio.h>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#define SCE_AUDIO_OUT_MAX_VOL 32768 //!< Maximum output port volume
#define SCE_AUDIO_VOLUME_0DB SCE_AUDIO_OUT_MAX_VOL //!< Maximum output port volume

typedef std::shared_ptr<SDL_AudioStream> AudioStreamPtr;
typedef std::function<void(SceUID)> ResumeAudioThread;

struct ReadOnlyAudioOutPortState {
    int len_bytes = 0;
};

enum SceAudioOutPortType {
    //! Used for main audio output, freq must be set to 48000 Hz
    SCE_AUDIO_OUT_PORT_TYPE_MAIN = 0,
    //! Used for Background Music port
    SCE_AUDIO_OUT_PORT_TYPE_BGM = 1,
    //! Used for voice chat port
    SCE_AUDIO_OUT_PORT_TYPE_VOICE = 2
};

struct SharedAudioOutPortState {
    std::mutex mutex;
    AudioStreamPtr stream;
    SceUID thread = -1;
};

struct AudioOutPort {
    ReadOnlyAudioOutPortState ro;
    SharedAudioOutPortState shared;
    // Channel range from 0 - 32768
    int left_channel_volume = SCE_AUDIO_VOLUME_0DB;
    int right_channel_volume = SCE_AUDIO_VOLUME_0DB;
    // Volume range from 1 - 128
    float volume = SDL_MIX_MAXVOLUME;
};

struct AudioInPort {
    SDL_AudioDeviceID id;
    bool running = false;
    int len_bytes = 0;
};

typedef std::shared_ptr<AudioOutPort> AudioOutPortPtr;
typedef std::map<int, AudioOutPortPtr> AudioOutPortPtrs;
typedef std::shared_ptr<void> AudioDevicePtr;

struct ReadOnlyAudioState {
    SDL_AudioSpec spec;
    ResumeAudioThread resume_thread;
};

struct AudioCallbackState {
    std::vector<uint8_t> temp_buffer;
};

struct SharedAudioState {
    std::mutex mutex;
    int next_port_id = 1;
    AudioOutPortPtrs out_ports;
    AudioInPort in_port;
    SceAudioOutPortType type;
};

struct AudioState {
    ReadOnlyAudioState ro;
    AudioCallbackState callback;
    SharedAudioState shared;
    AudioDevicePtr device;
};
