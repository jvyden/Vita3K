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

#include <ngs/modules/atrac9.h>
#include <ngs/modules/player.h>
#include <ngs/system.h>
#include <util/log.h>

#include "SceNgs.h"

#define SCE_NGS_MAX_SYSTEM_CHANNELS 2

using SceNgsSynthSystemHandle = Ptr<ngs::System>;
using SceNgsRackHandle = Ptr<ngs::Rack>;
using SceNgsVoiceHandle = Ptr<ngs::Voice>;
using SceNgsPatchHandle = Ptr<ngs::Patch>;

struct SceNgsVolumeMatrix {
    SceFloat32 matrix[SCE_NGS_MAX_SYSTEM_CHANNELS][SCE_NGS_MAX_SYSTEM_CHANNELS];
};

struct SceNgsPatchAudioPropInfo {
    SceInt32 out_channels;
    SceInt32 in_channels;
    SceNgsVolumeMatrix volume_matrix;
};

struct SceNgsVoiceInfo {
    SceUInt32 voice_state;
    SceUInt32 num_modules;
    SceUInt32 num_inputs;
    SceUInt32 num_outputs;
    SceUInt32 num_patches_per_output;
    SceUInt32 update_passed;
};

struct SceNgsVoicePreset {
    SceInt32 name_offset;
    SceUInt32 name_length;
    SceInt32 preset_data_offset;
    SceUInt32 size_preset_data;
    SceInt32 bypass_flags_offset;
    SceUInt32 num_bypass_blags;
};

static_assert(sizeof(SceNgsPatchAudioPropInfo) == 24);

struct SceNgsPatchDeliveryInfo {
    SceNgsVoiceHandle source_voice_handle;
    SceInt32 output_index;
    SceInt32 output_subindex;
    SceNgsVoiceHandle dest_voice_handle;
    SceInt32 input_index;
};

static_assert(sizeof(SceNgsPatchDeliveryInfo) == 20);

static constexpr SceUInt32 SCE_NGS_OK = 0;
static constexpr SceUInt32 SCE_NGS_ERROR = 0x804A0001;
static constexpr SceUInt32 SCE_NGS_ERROR_INVALID_ARG = 0x804A0002;
static constexpr SceUInt32 SCE_NGS_ERROR_INVALID_STATE = 0x804A0010;
static constexpr SceUInt32 SCE_NGS_ERROR_PARAM_OUT_OF_RANGE = 0x804A0009;
static constexpr SceUInt32 SCE_NGS_SIZE_MISMATCH = 0x804A000D;

static constexpr SceUInt32 SCE_NGS_VOICE_STATE_AVAILABLE = 0;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_ACTIVE = 1 << 0;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_FINALIZE = 1 << 2;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_UNLOADING = 1 << 3;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_PENDING = 1 << 4;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_PAUSED = 1 << 5;
static constexpr SceUInt32 SCE_NGS_VOICE_STATE_KEY_OFF = 1 << 6;

EXPORT(int, sceNgsAT9GetSectionDetails, const std::uint32_t samples_start, const std::uint32_t num_samples, const std::uint32_t config_data, ngs::atrac9::SkipBufferInfo *info) {
    if (host.cfg.current_config.disable_ngs) {
        return -1;
    }
    // Check magic!
    if ((config_data & 0xFF) != 0xFE && info) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    get_buffer_parameter(samples_start, num_samples, config_data, *info);
    return 0;
}

EXPORT(int, sceNgsModuleGetNumPresets) {
    return UNIMPLEMENTED();
}

EXPORT(SceInt32, sceNgsModuleGetPreset, SceNgsSynthSystemHandle sys_handle, const SceUInt32 uModuleID,
    const SceUInt32 uPresetIndex, ngs::BufferParamsInfo *pParamsBuffer) {
    LOG_DEBUG("module id: {}, preset index: {}", log_hex(uModuleID), uPresetIndex);
    ngs::System *system = sys_handle.get(host.mem);

    if (!system)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);

    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsPatchCreateRouting, ngs::PatchSetupInfo *patch_info, SceNgsPatchHandle *handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }
    assert(handle);

    // Make the scheduler order this right based on dependencies request
    ngs::Voice *source = patch_info->source.get(host.mem);

    if (!source) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    LOG_TRACE("Patching 0x{:X}:{}:{} to 0x{:X}:{}", patch_info->source.address(), patch_info->source_output_index,
        patch_info->source_output_index, patch_info->dest.address(), patch_info->dest_input_index);

    *handle = source->rack->system->voice_scheduler.patch(host.mem, patch_info);

    if (!*handle) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsPatchGetInfo, SceNgsPatchHandle patch_handle, SceNgsPatchAudioPropInfo *prop_info, SceNgsPatchDeliveryInfo *deli_info) {
    // Always stereo
    if (prop_info) {
        prop_info->in_channels = 2;
        prop_info->out_channels = 2;
    }

    if (host.cfg.current_config.disable_ngs)
        return SCE_NGS_OK;

    ngs::Patch *patch = patch_handle.get(host.mem);
    if (!patch) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (prop_info) {
        prop_info->volume_matrix.matrix[0][0] = patch->volume_matrix[0][0];
        prop_info->volume_matrix.matrix[0][1] = patch->volume_matrix[0][1];
        prop_info->volume_matrix.matrix[1][0] = patch->volume_matrix[1][0];
        prop_info->volume_matrix.matrix[1][1] = patch->volume_matrix[1][1];
    }

    if (deli_info) {
        deli_info->input_index = patch->dest_index;
        deli_info->output_index = patch->output_index;
        deli_info->output_subindex = patch->output_sub_index;
        deli_info->source_voice_handle = Ptr<ngs::Voice>(patch->source, host.mem);
        deli_info->dest_voice_handle = Ptr<ngs::Voice>(patch->dest, host.mem);
    }

    return SCE_NGS_OK;
}

EXPORT(int, sceNgsPatchRemoveRouting, SceNgsPatchHandle patch_handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Patch *patch = patch_handle.get(host.mem);

    if (!patch_handle.valid(host.mem) || !patch) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (!patch->source->remove_patch(host.mem, patch_handle)) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return 0;
}

EXPORT(int, sceNgsRackGetRequiredMemorySize, SceNgsSynthSystemHandle sys_handle, ngs::RackDescription *description, uint32_t *size) {
    if (host.cfg.current_config.disable_ngs) {
        *size = 1;
        return 0;
    }

    *size = ngs::Rack::get_required_memspace_size(host.mem, description);
    return 0;
}

EXPORT(SceUInt32, sceNgsRackGetVoiceHandle, SceNgsRackHandle rack_handle, const std::uint32_t index, SceNgsVoiceHandle *voice_handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }
    ngs::Rack *rack = rack_handle.get(host.mem);

    if (!rack || !voice_handle) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (index >= rack->voices.size()) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    *voice_handle = rack->voices[index];
    return SCE_NGS_OK;
}

EXPORT(SceUInt32, sceNgsRackInit, SceNgsSynthSystemHandle sys_handle, ngs::BufferParamsInfo *info, const ngs::RackDescription *description, SceNgsRackHandle *handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }
    assert(sys_handle);
    assert(info);
    assert(description);

    ngs::System *system = sys_handle.get(host.mem);

    if (!ngs::init_rack(host.ngs, host.mem, system, info, description)) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    *handle = info->data.cast<ngs::Rack>();
    return SCE_NGS_OK;
}

EXPORT(int, sceNgsRackRelease) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsRackSetParamErrorCallback) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsSystemGetRequiredMemorySize, ngs::SystemInitParameters *params, uint32_t *size) {
    if (host.cfg.current_config.disable_ngs) {
        *size = 1;
        return 0;
    }

    *size = ngs::System::get_required_memspace_size(params); // System struct size
    return 0;
}

EXPORT(SceUInt32, sceNgsSystemInit, Ptr<void> memspace, const std::uint32_t memspace_size, ngs::SystemInitParameters *params,
    SceNgsSynthSystemHandle *handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    if (!ngs::init_system(host.ngs, host.mem, params, memspace, memspace_size)) {
        return RET_ERROR(SCE_NGS_ERROR); // TODO: Better error code
    }

    *handle = memspace.cast<ngs::System>();
    return SCE_NGS_OK;
}

EXPORT(int, sceNgsSystemLock) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsSystemRelease) {
    return UNIMPLEMENTED();
}

EXPORT(SceInt32, sceNgsSystemSetFlags, SceNgsSynthSystemHandle *handle, const SceUInt32 flags) {
    //LOG_DEBUG("flags: {}", flags);
    //ngs::System *system = handle->get(host.mem);
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsSystemSetParamErrorCallback) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsSystemUnlock) {
    return UNIMPLEMENTED();
}

EXPORT(SceUInt32, sceNgsSystemUpdate, SceNgsSynthSystemHandle handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::System *sys = handle.get(host.mem);
    sys->voice_scheduler.update(host.kernel, host.mem, thread_id);

    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoiceBypassModule, SceNgsVoiceHandle voice_handle,
    const SceUInt32 module_id, const SceUInt32 bypass_flag) {
    //LOG_DEBUG("module id: {}, bypass flag: {}", module_id, bypass_flag);
    //ngs::Voice *voice = voice_handle.get(host.mem);
    //ngs::ModuleData *storage = voice->module_storage(module_id);

    return UNIMPLEMENTED();
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetAtrac9Voice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_ATRAC9);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetCompressorBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_COMPRESSOR);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetCompressorSideChainBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SIDE_CHAIN_COMPRESSOR);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetDelayBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_DELAY);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetDistortionBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_DISTORTION);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetEnvelopeBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_ENVELOPE);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetEqBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_EQUALIZATION);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetMasterBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_MASTER);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetMixerBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_MIXER);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetPauserBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_PAUSER);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetPitchShiftBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_PITCH_SHIFT);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetReverbBuss) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_REVERB);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetSasEmuVoice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SAS_EMULATION);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetScreamAtrac9Voice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SCREAM_ATRAC9);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetScreamVoice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SCREAM);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetSimpleAtrac9Voice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SIMPLE_ATRAC9);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetSimpleVoice) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_SIMPLE);
}

EXPORT(Ptr<ngs::VoiceDefinition>, sceNgsVoiceDefGetTemplate1) {
    if (host.cfg.current_config.disable_ngs) {
        return Ptr<ngs::VoiceDefinition>(0);
    }

    return get_voice_definition(host.ngs, host.mem, ngs::BussType::BUSS_NORMAL_PLAYER);
}

static SceUInt32 ngsVoiceStateFromHLEState(const ngs::VoiceState state) {
    switch (state) {
    case ngs::VoiceState::VOICE_STATE_AVAILABLE:
        return SCE_NGS_VOICE_STATE_AVAILABLE;

    case ngs::VoiceState::VOICE_STATE_ACTIVE:
        return SCE_NGS_VOICE_STATE_ACTIVE;

    case ngs::VoiceState::VOICE_STATE_FINALIZING:
        return SCE_NGS_VOICE_STATE_FINALIZE;

    case ngs::VoiceState::VOICE_STATE_KEY_OFF:
        return SCE_NGS_VOICE_STATE_KEY_OFF;

    case ngs::VoiceState::VOICE_STATE_PAUSED:
        return SCE_NGS_VOICE_STATE_PAUSED;

    case ngs::VoiceState::VOICE_STATE_PENDING:
        return SCE_NGS_VOICE_STATE_PENDING;

    case ngs::VoiceState::VOICE_STATE_UNLOADING:
        return SCE_NGS_VOICE_STATE_UNLOADING;

    default:
        assert(false && "Invalid voice state to translate");
        break;
    }

    return SCE_NGS_VOICE_STATE_AVAILABLE;
}

EXPORT(SceInt32, sceNgsVoiceGetInfo, SceNgsVoiceHandle handle, SceNgsVoiceInfo *info) {
    if (host.cfg.current_config.disable_ngs)
        return SCE_NGS_OK;

    ngs::Voice *voice = handle.get(host.mem);
    if (!voice || !info) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    const std::lock_guard<std::mutex> guard(*voice->voice_lock);

    info->voice_state = ngsVoiceStateFromHLEState(voice->state);
    info->num_modules = static_cast<SceUInt32>(voice->datas.size());
    info->num_inputs = static_cast<SceUInt32>(voice->inputs.inputs.size());
    info->num_outputs = voice->rack->vdef->output_count();
    info->num_patches_per_output = static_cast<SceUInt32>(voice->rack->patches_per_output);
    info->update_passed = voice->frame_count;

    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoiceGetModuleBypass) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceNgsVoiceGetModuleType) {
    return UNIMPLEMENTED();
}

EXPORT(SceInt32, sceNgsVoiceGetOutputPatch, SceNgsVoiceHandle voice_handle, const SceInt32 output_index, const SceInt32 output_subindex, SceNgsPatchHandle *handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);

    if (!voice || !handle) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if ((output_subindex < 0) || (output_index < 0)) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if ((output_index >= static_cast<SceInt32>(voice->rack->vdef->output_count())) || (output_subindex >= voice->rack->patches_per_output)) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    *handle = voice->patches[output_index][output_subindex];
    if (!(*handle) || (handle->get(host.mem))->output_sub_index == -1) {
        LOG_WARN("Getting non-existen output patch port {}:{}", output_index, output_subindex);
        *handle = nullptr;
    }

    return 0;
}

EXPORT(int, sceNgsVoiceGetParamsOutOfRange) {
    return UNIMPLEMENTED();
}

EXPORT(SceInt32, sceNgsVoiceGetStateData, SceNgsVoiceHandle voice_handle, const SceUInt32 module, void *mem, const SceUInt32 mem_size) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);
    ngs::ModuleData *storage = voice->module_storage(module);

    if (!storage) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    std::memcpy(mem, storage->voice_state_data.data(), std::min<std::size_t>(mem_size, storage->voice_state_data.size()));
    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoiceInit, SceNgsVoiceHandle voice_handle, const SceNgsVoicePreset *pPreset, const SceUInt32 uInitFlags) {
    if (host.cfg.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);

    if (!voice)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);

    LOG_DEBUG_IF(pPreset, "bypass: {}, name: {}", pPreset->bypass_flags_offset, pPreset->name_length);
    LOG_DEBUG("flags: {}", uInitFlags);
    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoiceKeyOff, SceNgsVoiceHandle voice_handle) {
    if (host.cfg.current_config.disable_ngs) {
        return SCE_NGS_OK;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);

    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    voice->rack->system->voice_scheduler.off(voice);
    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoiceKill, SceNgsVoiceHandle voice_handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);

    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    voice->rack->system->voice_scheduler.stop(voice);
    return 0;
}

EXPORT(SceUInt32, sceNgsVoiceLockParams, SceNgsVoiceHandle voice_handle, SceUInt32 module, SceUInt32 unk2, Ptr<ngs::BufferParamsInfo> buf) {
    if (host.cfg.current_config.disable_ngs) {
        auto *buffer_info = buf.get(host.mem);

        buffer_info->data = alloc(host.mem, 10, "SceNgs buffer stub");
        buffer_info->size = 10;
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);
    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    ngs::ModuleData *data = voice->module_storage(module);

    if (!data) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    ngs::BufferParamsInfo *info = data->lock_params(host.mem);

    if (!info) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    *(buf.get(host.mem)) = *info;
    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoicePatchSetVolume, SceNgsPatchHandle patch_handle, const SceInt32 output_channel, const SceInt32 input_channel, const SceFloat32 vol) {
    if (host.cfg.current_config.disable_ngs)
        return SCE_NGS_OK;

    ngs::Patch *patch = patch_handle.get(host.mem);
    if (!patch || patch->output_sub_index == -1)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    //LOG_DEBUG("out chanel: {}, in chanel: {}, vol: {}", output_channel, input_channel, vol);
    patch->volume_matrix[output_channel][input_channel] = vol;

    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoicePatchSetVolumes, SceNgsPatchHandle patch_handle, const SceInt32 output_channel, const SceFloat32 *volumes, const SceInt32 vols) {
    if (host.cfg.current_config.disable_ngs)
        return SCE_NGS_OK;

    ngs::Patch *patch = patch_handle.get(host.mem);
    if (!patch || patch->output_sub_index == -1)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);

    //for (int i = 0; i < std::min(vols, 2); i++)
    //     patch->volume_matrix[output_channel][i] = volumes[i];

    LOG_DEBUG("End, out chanel: {}, (volumes, 0: {}, 1: {}), vols: {}", output_channel, volumes[0], volumes[1], vols);

    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoicePatchSetVolumesMatrix, SceNgsPatchHandle patch_handle, const SceNgsVolumeMatrix *matrix) {
    if (host.cfg.current_config.disable_ngs)
        return 0;

    ngs::Patch *patch = patch_handle.get(host.mem);
    if (!patch || patch->output_sub_index == -1)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);

    patch->volume_matrix[0][0] = matrix->matrix[0][0];
    patch->volume_matrix[0][1] = matrix->matrix[0][1];
    patch->volume_matrix[1][0] = matrix->matrix[1][0];
    patch->volume_matrix[1][1] = matrix->matrix[1][1];

    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoicePause, SceNgsVoiceHandle handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = handle.get(host.mem);

    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (!voice->rack->system->voice_scheduler.pause(voice)) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return SCE_NGS_OK;
}

EXPORT(SceUInt32, sceNgsVoicePlay, SceNgsVoiceHandle handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = handle.get(host.mem);
    LOG_DEBUG("sceNgsVoicePlay: {}", voice->state);
    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (!voice->rack->system->voice_scheduler.play(host.mem, voice)) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoiceResume, SceNgsVoiceHandle handle) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = handle.get(host.mem);

    if (!voice) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    LOG_DEBUG("Resume: {}", voice->state);
    if (voice->state != ngs::VOICE_STATE_PAUSED)
        //return RET_ERROR(SCE_NGS_ERROR_INVALID_STATE);
        return SCE_NGS_OK;

    if (!voice->rack->system->voice_scheduler.resume(host.mem, voice)) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return SCE_NGS_OK;
}

EXPORT(SceInt32, sceNgsVoiceSetFinishedCallback, SceNgsVoiceHandle voice_handle, Ptr<ngs::ModuleCallback> callback, Ptr<void> user_data) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);
    if (!voice)
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);

    return UNIMPLEMENTED();
}

EXPORT(SceInt32, sceNgsVoiceSetModuleCallback, SceNgsVoiceHandle voice_handle, const SceUInt32 module, Ptr<ngs::ModuleCallback> callback, Ptr<void> user_data) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = voice_handle.get(host.mem);
    ngs::ModuleData *storage = voice->module_storage(module);
    if (!storage) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    storage->callback = callback;
    storage->user_data = user_data;

    return 0;
}

enum ModuleIdentifier {
    SCE_NGS_AT9_ID = 0x01015CAA, // Specifies the ATRAC9™ Player ID.
    SCE_NGS_COMPRESSOR_ID = 0x01015CE1, //Specifies the Compressor ID.
    SCE_NGS_COMPRESSOR_ID_V2 = 0x01025CE1, //Specifies the Compressor ID.
    SCE_NGS_DELAY_ID = 0x01015CEB, // Specifies the Delay ID.
    SCE_NGS_DISTORTION_ID = 0x01015CE2, //Specifies the Distortion ID.
    SCE_NGS_ENVELOPE_ID = 0x01015CE3, //Specifies the Envelope ID.
    SCE_NGS_FILTER_ID = 0x01015CE4, //Specifies the Filter ID.
    SCE_NGS_FILTER_ID_2 = 0x02015CE4, //Specifies the Filter ID.
    SCE_NGS_INPUT_MIXER_ID = 0x01015CE0, //Specifies the Input Mixer ID.
    SCE_NGS_MIXER_ID = 0x01015CE9, //Specifies the Mixer ID.
    SCE_NGS_OUTPUT_ID = 0x01015CED, //Specifies the Output ID. The output module returns a user
    //state block of PCM samples. The size of the block represents
    //the number of channels (output’s voice) * system granularity *
    //sizeof(short).
    SCE_NGS_PARAM_EQ_ID = 0x01015CEC, //Specifies the Parametric EQ ID.
    SCE_NGS_PARAM_EQ_ID_2 = 0x02015CEC, //Specifies the Parametric EQ ID 2.
    SCE_NGS_PAUSER_ID = 0x01015CE5, //Specifies the Pauser ID.
    SCE_NGS_PLAYER_ID = 0x01015CE6, //Specifies the Player ID.
    SCE_NGS_PITCHSHIFT_ID = 0x5CEA, //Specifies the Pitch Shift ID.
    SCE_NGS_REVERB_ID = 0x01015CE7, //Specifies the Reverb ID.
    SCE_NGS_REVERB_ID_2 = 0x01025CE7, //Specifies the Reverb ID.
    SCE_NGS_GENERATOR_ID = 0x01015CE8, // Specifies the Signal Generator ID.
};

// crée une function qui retourne le nom en string du module par id (id est un SceUInt32) a base
// de la table ModuleIdentifier
std::string get_module_name(SceUInt32 id) {
    switch (id) {
    case SCE_NGS_AT9_ID:
        return "ATRAC9";
    case SCE_NGS_COMPRESSOR_ID:
    case SCE_NGS_COMPRESSOR_ID_V2:
        return "COMPRESSOR";
    case SCE_NGS_DELAY_ID:
        return "DELAY";
    case SCE_NGS_DISTORTION_ID:
        return "DISTORTION";
    case SCE_NGS_ENVELOPE_ID:
        return "ENVELOPE";
    case SCE_NGS_FILTER_ID:
    case SCE_NGS_FILTER_ID_2:
        return "FILTER";
    case SCE_NGS_INPUT_MIXER_ID:
        return "INPUT_MIXER";
    case SCE_NGS_MIXER_ID:
        return "MIXER";
    case SCE_NGS_OUTPUT_ID:
        return "OUTPUT";
    case SCE_NGS_PARAM_EQ_ID:
        return "PARAM_EQ";
    case SCE_NGS_PARAM_EQ_ID_2:
        return "PARAM_EQ 2";
    case SCE_NGS_PAUSER_ID:
        return "PAUSER";
    case SCE_NGS_PLAYER_ID:
        return "PLAYER";
    case SCE_NGS_PITCHSHIFT_ID:
        return "PITCHSHIFT";
    case SCE_NGS_REVERB_ID:
    case SCE_NGS_REVERB_ID_2:
        return "REVERB";
    case SCE_NGS_GENERATOR_ID:
        return "GENERATOR";
    default:
        return fmt::format("UNKNOWN: {}", log_hex(id));
    }
}

EXPORT(SceInt32, sceNgsVoiceSetParamsBlock, SceNgsVoiceHandle voice_handle, const ngs::ModuleParameterHeader *header,
    const SceUInt32 size, SceInt32 *num_error) {
    if (host.cfg.current_config.disable_ngs)
        return SCE_NGS_OK;

    ngs::Voice *voice = voice_handle.get(host.mem);

    const std::lock_guard<std::mutex> guard(*voice->voice_lock);

    const SceUInt8 *data = reinterpret_cast<const SceUInt8 *>(header);
    const SceUInt8 *data_end = reinterpret_cast<const SceUInt8 *>(data + size);

    // after first loop, check if other module exist
    while (data < data_end) {
        // init Storage with module index give by app
        ngs::ModuleData *storage = voice->module_storage(header->module_id);

        // incremente data with size of struct header for go inside struct of current module
        data += sizeof(ngs::ModuleParameterHeader);

        // init descriptor parameters
        const auto *desc = reinterpret_cast<const ngs::ParametersDescriptor *>(data);
        const auto module_name = get_module_name(desc->id);
        //LOG_INFO("module id: {}, module name: {}, size: {}", header->module_id, module_name, desc->size);

        // copy this current module inside current buffer data when it available with using descriptor size
        if (storage) {
            SceUInt8 *param_ptr = reinterpret_cast<SceUInt8 *>(storage->info.data.get(host.mem));
            /*if (module_name == "PLAYER") {
                const auto *get_params = reinterpret_cast<const ngs::player::Parameters *>(data);
                LOG_DEBUG("playback, freq: {}, scal: {}", get_params->playback_frequency, get_params->playback_scalar);
                for (auto i = 0; i < 4; i++) {
                    const auto current_buffer = get_params->buffer_params[i];
                    LOG_DEBUG("buffer {}, next buff: {}, numbytes: {}, loop count: {}", i, current_buffer.next_buffer_index, current_buffer.bytes_count, current_buffer.loop_count);
                }
            } else if (module_name == "ATRAC9") {
                const auto *get_params = reinterpret_cast<const ngs::atrac9::Parameters *>(data);
                LOG_DEBUG("playback, freq: {}, scal: {}", get_params->playback_frequency, get_params->playback_scalar);
                for (auto i = 0; i < 4; i++) {
                    const auto current_buffer = get_params->buffer_params[i];
                    LOG_DEBUG("buffer {}, next buff: {}, numbytes: {}, loop count: {}", i, current_buffer.next_buffer_index, current_buffer.bytes_count, current_buffer.loop_count);
                }
            }*/
            memcpy(param_ptr, data, desc->size);
        } else
            ++num_error;

        // increment using the size of the descriptor of the current module to be able to move on to the next module when it exists
        data += desc->size;

        // set new header for can using on next module
        header = reinterpret_cast<const ngs::ModuleParameterHeader *>(data);
    }

    return SCE_NGS_OK;
}

EXPORT(int, sceNgsVoiceSetPreset) {
    return UNIMPLEMENTED();
}

EXPORT(SceUInt32, sceNgsVoiceUnlockParams, SceNgsVoiceHandle handle, const SceUInt32 module_index) {
    if (host.cfg.current_config.disable_ngs) {
        return 0;
    }

    ngs::Voice *voice = handle.get(host.mem);
    ngs::ModuleData *data = voice->module_storage(module_index);

    if (!data) {
        return RET_ERROR(SCE_NGS_ERROR_INVALID_ARG);
    }

    if (!data->unlock_params()) {
        return RET_ERROR(SCE_NGS_ERROR);
    }

    return SCE_NGS_OK;
}

EXPORT(int, sceSulphaNgsGetDefaultConfig) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsGetNeededMemory) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsInit) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsSetRackName) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsSetSampleName) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsSetSynthName) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsSetVoiceName) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsShutdown) {
    return UNIMPLEMENTED();
}

EXPORT(int, sceSulphaNgsTrace) {
    return UNIMPLEMENTED();
}

BRIDGE_IMPL(sceNgsAT9GetSectionDetails)
BRIDGE_IMPL(sceNgsModuleGetNumPresets)
BRIDGE_IMPL(sceNgsModuleGetPreset)
BRIDGE_IMPL(sceNgsPatchCreateRouting)
BRIDGE_IMPL(sceNgsPatchGetInfo)
BRIDGE_IMPL(sceNgsPatchRemoveRouting)
BRIDGE_IMPL(sceNgsRackGetRequiredMemorySize)
BRIDGE_IMPL(sceNgsRackGetVoiceHandle)
BRIDGE_IMPL(sceNgsRackInit)
BRIDGE_IMPL(sceNgsRackRelease)
BRIDGE_IMPL(sceNgsRackSetParamErrorCallback)
BRIDGE_IMPL(sceNgsSystemGetRequiredMemorySize)
BRIDGE_IMPL(sceNgsSystemInit)
BRIDGE_IMPL(sceNgsSystemLock)
BRIDGE_IMPL(sceNgsSystemRelease)
BRIDGE_IMPL(sceNgsSystemSetFlags)
BRIDGE_IMPL(sceNgsSystemSetParamErrorCallback)
BRIDGE_IMPL(sceNgsSystemUnlock)
BRIDGE_IMPL(sceNgsSystemUpdate)
BRIDGE_IMPL(sceNgsVoiceBypassModule)
BRIDGE_IMPL(sceNgsVoiceDefGetAtrac9Voice)
BRIDGE_IMPL(sceNgsVoiceDefGetCompressorBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetCompressorSideChainBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetDelayBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetDistortionBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetEnvelopeBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetEqBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetMasterBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetMixerBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetPauserBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetPitchShiftBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetReverbBuss)
BRIDGE_IMPL(sceNgsVoiceDefGetSasEmuVoice)
BRIDGE_IMPL(sceNgsVoiceDefGetScreamAtrac9Voice)
BRIDGE_IMPL(sceNgsVoiceDefGetScreamVoice)
BRIDGE_IMPL(sceNgsVoiceDefGetSimpleAtrac9Voice)
BRIDGE_IMPL(sceNgsVoiceDefGetSimpleVoice)
BRIDGE_IMPL(sceNgsVoiceDefGetTemplate1)
BRIDGE_IMPL(sceNgsVoiceGetInfo)
BRIDGE_IMPL(sceNgsVoiceGetModuleBypass)
BRIDGE_IMPL(sceNgsVoiceGetModuleType)
BRIDGE_IMPL(sceNgsVoiceGetOutputPatch)
BRIDGE_IMPL(sceNgsVoiceGetParamsOutOfRange)
BRIDGE_IMPL(sceNgsVoiceGetStateData)
BRIDGE_IMPL(sceNgsVoiceInit)
BRIDGE_IMPL(sceNgsVoiceKeyOff)
BRIDGE_IMPL(sceNgsVoiceKill)
BRIDGE_IMPL(sceNgsVoiceLockParams)
BRIDGE_IMPL(sceNgsVoicePatchSetVolume)
BRIDGE_IMPL(sceNgsVoicePatchSetVolumes)
BRIDGE_IMPL(sceNgsVoicePatchSetVolumesMatrix)
BRIDGE_IMPL(sceNgsVoicePause)
BRIDGE_IMPL(sceNgsVoicePlay)
BRIDGE_IMPL(sceNgsVoiceResume)
BRIDGE_IMPL(sceNgsVoiceSetFinishedCallback)
BRIDGE_IMPL(sceNgsVoiceSetModuleCallback)
BRIDGE_IMPL(sceNgsVoiceSetParamsBlock)
BRIDGE_IMPL(sceNgsVoiceSetPreset)
BRIDGE_IMPL(sceNgsVoiceUnlockParams)
BRIDGE_IMPL(sceSulphaNgsGetDefaultConfig)
BRIDGE_IMPL(sceSulphaNgsGetNeededMemory)
BRIDGE_IMPL(sceSulphaNgsInit)
BRIDGE_IMPL(sceSulphaNgsSetRackName)
BRIDGE_IMPL(sceSulphaNgsSetSampleName)
BRIDGE_IMPL(sceSulphaNgsSetSynthName)
BRIDGE_IMPL(sceSulphaNgsSetVoiceName)
BRIDGE_IMPL(sceSulphaNgsShutdown)
BRIDGE_IMPL(sceSulphaNgsTrace)
