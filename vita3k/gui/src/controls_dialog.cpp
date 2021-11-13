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

#include "private.h"

#include <config/functions.h>
#include <ctrl/ctrl.h>
#include <gui/functions.h>
#include <host/state.h>
#include <interface.h>

#include <stb_image.h>

namespace gui {

static char const *SDL_key_to_string[]{ "[unset]", "[unknown]", "[unknown]", "[unknown]", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "Return", "Escape", "Backspace", "Tab", "Space", "-", "=", "[", "]", "\\", "NonUS #", ";", "'", "Grave", ",", ".", "/", "CapsLock", "F1",
    "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "PrtScrn", "ScrlLock", "Pause", "Insert", "Home", "PgUp", "Delete",
    "End", "PgDown", "Ar Right", "Ar Left", "Ar Down", "Ar Up", "NumLock/Clear", "Keypad /", "Keypad *", "Keypad -", "Keypad +",
    "Keypad Enter", "Keypad 1", "Keypad 2", "Keypad 3", "Keypad 4", "Keypad 5", "Keypad 6", "Keypad 7", "Keypad 8", "Keypad 9", "Keypad 0",
    "Keypad .", "NonUs \\", "App", "Power", "Keypad =", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "Execute",
    "Help", "Menu", "Select", "Stop", "Again", "Undo", "Cut", "Copy", "Paste", "Find", "Mute", "VolUp", "VolDown", "[unset]", "[unset]", "[unset]",
    "Keypad ,", "Kp = As400", "International1", "International2", "International3", "International4", "International5", "International6",
    "International7", "International8", "International9", "Lang1", "Lang2", "Lang3", "Lang4", "Lang5", "Lang6", "Lang7", "Lang8", "Lang9", "Alt Erase",
    "SysReq", "Cancel", "Clear", "Prior", "Return2", "Separator", "Out", "Oper", "ClearAgain", "Crsel", "Exsel", "[unset]", "[unset]", "[unset]",
    "[unset]", "[unset]", "[unset]", "[unset]", "[unset]", "[unset]", "[unset]", "[unset]", "Keypad 00", "Keypad 000", "ThousSeparat", "DecSeparat",
    "CurrencyUnit", "CurrencySubUnit", "Keypad (", "Keypad )", "Keypad {", "Keypad }", "Keypad Tab", "Keypad Backspace", "Keypad A", "Keypad B",
    "Keypad C", "Keypad D", "Keypad E", "Keypad F", "Keypad XOR", "Keypad Power", "Keypad %", "Keypad <", "Keypad >", "Keypad &", "Keypad &&",
    "Keypad |", "Keypad ||", "Keypad :", "Keypad #", "Keypad Space", "Keypad @", "Keypad !", "Keypad MemStr", "Keypad MemRec", "Keypad MemClr",
    "Keypad Mem+", "Keypad Mem-", "Keypad Mem*", "Keypad Mem/", "Keypad +/-", "Keypad Clear", "Keypad ClearEntry", "Keypad Binary", "Keypad Octal",
    "Keypad Dec", "Keypad HexaDec", "[unset]", "[unset]", "LCtrl", "LShift", "LAlt", "Win/Cmd", "RCtrl", "RShift", "RAlt", "RWin/Cmd" };

// Crée une liste vector SceCtrlButtons de tout les boutton
static std::vector<SceCtrlButtons> list_buttons {
    SCE_CTRL_SELECT, SCE_CTRL_L3, SCE_CTRL_R3, SCE_CTRL_START, 
    SCE_CTRL_UP, SCE_CTRL_RIGHT, SCE_CTRL_DOWN, SCE_CTRL_LEFT,
    SCE_CTRL_L2, SCE_CTRL_R2, SCE_CTRL_L1, SCE_CTRL_R1,
    SCE_CTRL_TRIANGLE, SCE_CTRL_CIRCLE, SCE_CTRL_CROSS, SCE_CTRL_SQUARE,
    SCE_CTRL_PSBUTTON
};

//static vector<SceCtrlButtons> 
static int *get_config_button(Config &cfg, SceCtrlButtons &button) {
    switch (button) {
    case SCE_CTRL_SELECT:
        return &cfg.keyboard_button_select;
    case SCE_CTRL_START:
        return &cfg.keyboard_button_start;
    case SCE_CTRL_UP:
        return &cfg.keyboard_button_up;
    case SCE_CTRL_RIGHT:
        return &cfg.keyboard_button_right;
    case SCE_CTRL_DOWN:
        return &cfg.keyboard_button_down;
    case SCE_CTRL_LEFT:
        return &cfg.keyboard_button_left;
    case SCE_CTRL_TRIANGLE:
        return &cfg.keyboard_button_triangle;
    case SCE_CTRL_CIRCLE:
        return &cfg.keyboard_button_circle;
    case SCE_CTRL_CROSS:
        return &cfg.keyboard_button_cross;
    case SCE_CTRL_SQUARE:
        return &cfg.keyboard_button_square;
    case SCE_CTRL_L1:
        return &cfg.keyboard_button_l1;
    case SCE_CTRL_R1:
        return &cfg.keyboard_button_r1;
    case SCE_CTRL_L2:
        return &cfg.keyboard_button_l2;
    case SCE_CTRL_R2:
        return &cfg.keyboard_button_r2;
    case SCE_CTRL_L3:
        return &cfg.keyboard_button_l3;
    case SCE_CTRL_R3:
        return &cfg.keyboard_button_r3;
    default: break;
    }
}

static ImVec2 get_pos_button(SceCtrlButtons button) {
    const auto BTN_CENTER = ImVec2(218.f, 60.f);
    const auto DPAD_CENTER = ImVec2(-270.f, 60.f);
    switch (button) {
    //case SCE_CTRL_SELECT:
    //    return &cfg.keyboard_button_select;
    //case SCE_CTRL_START:
    //    return &cfg.keyboard_button_start;
    case SCE_CTRL_UP:
        return ImVec2(DPAD_CENTER.x, DPAD_CENTER.y + 44.f);
    case SCE_CTRL_RIGHT:
        return ImVec2(DPAD_CENTER.x + 64.f, DPAD_CENTER.y);
    case SCE_CTRL_DOWN:
        return ImVec2(DPAD_CENTER.x, DPAD_CENTER.y - 44.f);
    case SCE_CTRL_LEFT:
        return ImVec2(DPAD_CENTER.x - 64.f, DPAD_CENTER.y);
    case SCE_CTRL_TRIANGLE:
        return ImVec2(BTN_CENTER.x, BTN_CENTER.y + 48.f);
    case SCE_CTRL_CIRCLE:
        return ImVec2(BTN_CENTER.x + 50.f, BTN_CENTER.y);
    case SCE_CTRL_CROSS:
        return ImVec2(BTN_CENTER.x, BTN_CENTER.y - 48.f);
    case SCE_CTRL_SQUARE:
        return ImVec2(BTN_CENTER.x - 50.f, BTN_CENTER.y);
    /*case SCE_CTRL_L1:
        return &cfg.keyboard_button_l1;
    case SCE_CTRL_R1:
        return &cfg.keyboard_button_r1;
    case SCE_CTRL_L2:
        return &cfg.keyboard_button_l2;
    case SCE_CTRL_R2:
        return &cfg.keyboard_button_r2;
    case SCE_CTRL_L3:
        return &cfg.keyboard_button_l3;
    case SCE_CTRL_R3:
        return &cfg.keyboard_button_r3;*/
    default: return ImVec2(0,0); break;
    }
}

static void remapper_button(GuiState &gui, HostState &host, SceCtrlButtons ctrlbutton, float scale) {
    const auto button = get_config_button(host.cfg, ctrlbutton);
    if (ImGui::Button(SDL_key_to_string[*button], ImVec2(56.f * scale, 0.f))) {
        gui.old_captured_key = *button;
        gui.is_capturing_keys = true;
        while (gui.is_capturing_keys) {
            handle_events(host, gui);
            *button = gui.captured_key;
            if (*button < 0 || *button > 231)
                *button = 0;
        }
        config::serialize_config(host.cfg, host.cfg.config_path);
    }
}

void draw_controls_dialog(GuiState &gui, HostState &host) {
    const auto display_size = ImGui::GetIO().DisplaySize;
    const auto RES_SCALE = ImVec2(display_size.x / host.res_width_dpi_scale, display_size.y / host.res_height_dpi_scale);
    const auto SCALE = ImVec2(RES_SCALE.x * host.dpi_scale, RES_SCALE.y * host.dpi_scale);
    ImGui::SetNextWindowSize(ImVec2(700.f * SCALE.x, 380.f * SCALE.y));
    ImGui::SetNextWindowPos(ImVec2(display_size.x / 2.f, display_size.y / 2.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Controls", &gui.controls_menu.controls_dialog, ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(0.8f * RES_SCALE.x);
    const ImU32 WHITE = 0xFFFFFFFF;
    const auto CENTER = ImVec2(display_size.x / 2, display_size.y / 2);
    const auto VITA_MIN = ImVec2(CENTER.x - (300.f * SCALE.x), CENTER.y - (136.f * SCALE.x));    
    const auto VITA_MAX = ImVec2(CENTER.x + (300.f * SCALE.x), CENTER.y + (136.f * SCALE.x));
    const auto VITA_ROUND = 120.0f * SCALE.x; 
    const auto UP_CIRCLE = ImVec2(242.f * SCALE.x, 48.f * SCALE.y);
    const auto BUTTON_CENTER = ImVec2(CENTER.x + UP_CIRCLE.x, CENTER.y - UP_CIRCLE.y);

    const auto ANALOG = ImVec2(226.f * SCALE.x, 28.f * SCALE.x);
    // Draw black font
    ImGui::GetWindowDrawList()->AddRectFilled(VITA_MIN, VITA_MAX, IM_COL32(0.f, 0.f, 0.f, 175.f), VITA_ROUND, ImDrawFlags_RoundCornersAll);
    // Draw cirlce of D-Pad
    const auto D_PAD_CENTER = ImVec2(CENTER.x - UP_CIRCLE.x, CENTER.y - UP_CIRCLE.y);
    ImGui::GetWindowDrawList()->AddCircle(D_PAD_CENTER, 42.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddCircle(D_PAD_CENTER, 38.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    // Draw D-Pad
    // Draw Up
    const auto UP_CENTER = ImVec2(D_PAD_CENTER.x , D_PAD_CENTER.y - (22.f * SCALE.y));
    ImGui::GetWindowDrawList()->AddTriangle(ImVec2(UP_CENTER.x - (6.f * SCALE.x), UP_CENTER.y + (5.f * SCALE.y)),
        ImVec2(UP_CENTER.x, UP_CENTER.y - (5.f * SCALE.y)),
        ImVec2(UP_CENTER.x + (6.f * SCALE.x), UP_CENTER.y + (5.f * SCALE.y)), WHITE, 2.f * SCALE.x);
    // Draw Up
    const auto DOWN_CENTER = ImVec2(D_PAD_CENTER.x, D_PAD_CENTER.y + (22.f * SCALE.y));
    ImGui::GetWindowDrawList()->AddTriangle(ImVec2(DOWN_CENTER.x - (6.f * SCALE.x), DOWN_CENTER.y - (5.f * SCALE.y)),
        ImVec2(DOWN_CENTER.x, DOWN_CENTER.y + (5.f * SCALE.y)),
        ImVec2(DOWN_CENTER.x + (6.f * SCALE.x), DOWN_CENTER.y - (5.f * SCALE.y)), WHITE, 2.f * SCALE.x);
    // Draw Left
    const auto LEFT_CENTER = ImVec2(D_PAD_CENTER.x - (22.f * SCALE.x), D_PAD_CENTER.y);
    ImGui::GetWindowDrawList()->AddTriangle(ImVec2(LEFT_CENTER.x + (5.f * SCALE.x), LEFT_CENTER.y - (6.f * SCALE.y)),
        ImVec2(LEFT_CENTER.x - (5.f * SCALE.x), LEFT_CENTER.y),
        ImVec2(LEFT_CENTER.x + (5.f * SCALE.x), LEFT_CENTER.y + (6.f * SCALE.y)), WHITE, 2.f * SCALE.x);
    // Draw Right
    const auto RIGHT_CENTER = ImVec2(D_PAD_CENTER.x + (22.f * SCALE.x), D_PAD_CENTER.y);
    ImGui::GetWindowDrawList()->AddTriangle(ImVec2(RIGHT_CENTER.x - (5.f * SCALE.x), RIGHT_CENTER.y - (6.f * SCALE.y)),
        ImVec2(RIGHT_CENTER.x + (5.f * SCALE.x), RIGHT_CENTER.y),
        ImVec2(RIGHT_CENTER.x - (5.f * SCALE.x), RIGHT_CENTER.y + (6.f * SCALE.y)), WHITE, 2.f * SCALE.x);
    // Draw cirlce of Left Analog
    const auto LEFT_ANALOG_CENTER = ImVec2(CENTER.x - ANALOG.x, CENTER.y + ANALOG.y);
    ImGui::GetWindowDrawList()->AddCircle(LEFT_ANALOG_CENTER, 28.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    // Draw PS
    const auto PS = ImVec2(LEFT_ANALOG_CENTER.x - (28.f * SCALE.x), LEFT_ANALOG_CENTER.y + (40.f * SCALE.y));
    ImGui::GetWindowDrawList()->AddRect(PS, ImVec2(PS.x + (44.f * SCALE.x), PS.y + (24.f * SCALE.y)),
        WHITE, 180.f * SCALE.x, ImDrawFlags_RoundCornersAll, 1.5f * SCALE.x);
    ImGui::GetWindowDrawList()->AddText(gui.vita_font, 14.f * SCALE.x, ImVec2(PS.x + (14.f * SCALE.x), PS.y + (6.f * SCALE.y)), WHITE, "PS");
    // Draw Right Analog
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(CENTER.x - ANALOG.x, CENTER.y + ANALOG.y), 18.f * SCALE.x, WHITE, 0, 3.f * SCALE.x);
    // Draw contourn of Screen
    ImGui::GetWindowDrawList()->AddRect(ImVec2(CENTER.x - (182.f * SCALE.x), CENTER.y - (118.f * SCALE.y)),
        ImVec2(CENTER.x + (182.f * SCALE.x), CENTER.y + (90.f * SCALE.y)), WHITE,
        0.f * SCALE.x, ImDrawFlags_RoundCornersAll, 2.f * SCALE.x);
    // Draw cirlce contourn of Button
    ImGui::GetWindowDrawList()->AddCircle(BUTTON_CENTER, 42.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    // Draw square Button
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(BUTTON_CENTER.x - (23.f * SCALE.x), BUTTON_CENTER.y), 12.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddRect(ImVec2(BUTTON_CENTER.x - (28.f * SCALE.x), BUTTON_CENTER.y - 10.f),
        ImVec2(BUTTON_CENTER.x - (18.f * SCALE.x), BUTTON_CENTER.y + (6.f * SCALE.y)),
        IM_COL32(255.f, 105.f, 248.f, 255.f), 0.f, ImDrawFlags_RoundCornersAll, 2.f * SCALE.x);
    // Draw cirlce Button
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(BUTTON_CENTER.x + (26.f * SCALE.x), BUTTON_CENTER.y), 12.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(BUTTON_CENTER.x + (26.f * SCALE.x), BUTTON_CENTER.y), 7.f * SCALE.x,
        IM_COL32(255, 102, 102, 255.f), 0, 3.f * SCALE.x);
    // Draw Triangle Button
    const auto TRIANGLE_CENTER = ImVec2(BUTTON_CENTER.x + (2.f * SCALE.x), BUTTON_CENTER.y - (23.5f * SCALE.y));
    ImGui::GetWindowDrawList()->AddCircle(TRIANGLE_CENTER, 12.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddTriangle(ImVec2(TRIANGLE_CENTER.x - (6.f * SCALE.x), TRIANGLE_CENTER.y + (5.f * SCALE.y)),
        ImVec2(TRIANGLE_CENTER.x, TRIANGLE_CENTER.y - (5.f * SCALE.y)),
        ImVec2(TRIANGLE_CENTER.x + (6.f * SCALE.x), TRIANGLE_CENTER.y + (5.f * SCALE.y)), IM_COL32(64, 226, 160, 255.f), 2.f * SCALE.x);
    // Draw cross Button
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(BUTTON_CENTER.x + (2.f * SCALE.x), BUTTON_CENTER.y + (24.f * SCALE.y)), 12.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddLine(ImVec2(BUTTON_CENTER.x - (4.f * SCALE.x), BUTTON_CENTER.y + (16.f * SCALE.y)),
        ImVec2(BUTTON_CENTER.x + (8.f * SCALE.x), BUTTON_CENTER.y + (30.f * SCALE.y)), IM_COL32(124, 178, 232, 255.f), 3.f * SCALE.x);
    ImGui::GetWindowDrawList()->AddLine(ImVec2(BUTTON_CENTER.x + (7.f * SCALE.x), BUTTON_CENTER.y + (16.f * SCALE.y)),
        ImVec2(BUTTON_CENTER.x - (4.f * SCALE.x), BUTTON_CENTER.y + (30.f * SCALE.y)), IM_COL32(124, 178, 232, 255.f), 3.f * SCALE.x);
    // Draw cirlce of Right Analog
    const auto RIGHT_ANALOG_CENTER = ImVec2(CENTER.x + ANALOG.x, CENTER.y + ANALOG.y);
    ImGui::GetWindowDrawList()->AddCircle(RIGHT_ANALOG_CENTER, 28.f * SCALE.x, WHITE, 0, 2.f * SCALE.x);
    // Draw Right Analog
    ImGui::GetWindowDrawList()->AddCircle(RIGHT_ANALOG_CENTER, 18.f * SCALE.x, WHITE, 0, 3.f * SCALE.x);
    // Draw Select
    const auto SELECT = ImVec2(RIGHT_ANALOG_CENTER.x - (28.f * SCALE.x), RIGHT_ANALOG_CENTER.y + (42.f * SCALE.y));
    ImGui::GetWindowDrawList()->AddRect(SELECT, ImVec2(SELECT.x + (30.f * SCALE.x), SELECT.y + (16.f * SCALE.y)),
        WHITE, 180.f * SCALE.x, ImDrawFlags_RoundCornersAll, 1.5f * SCALE.x);
    ImGui::GetWindowDrawList()->AddText(gui.vita_font, 8.5f * SCALE.x, ImVec2(SELECT.x + (2.f * SCALE.x), SELECT.y + (4.f * SCALE.x)), WHITE, "Select");
    // Draw Start
    const auto START = ImVec2(RIGHT_ANALOG_CENTER.x + (10.f * SCALE.x), RIGHT_ANALOG_CENTER.y + (42.f * SCALE.y));
    ImGui::GetWindowDrawList()->AddRect(START, ImVec2(START.x + (30.f * SCALE.x), START.y + (16.f * SCALE.y)),
        WHITE, 180.f * SCALE.x, ImDrawFlags_RoundCornersAll, 1.5f * SCALE.x);
    ImGui::GetWindowDrawList()->AddText(gui.vita_font, 8.5f * SCALE.x, ImVec2(START.x + (5.f * SCALE.x), START.y + (4.f * SCALE.y)), WHITE, "Start");
    // Draw contourn
    ImGui::GetWindowDrawList()->AddRect(VITA_MIN, VITA_MAX, WHITE, VITA_ROUND, ImDrawFlags_RoundCornersAll, 2.f * SCALE.x);
    
    // Draw All Button
    const auto WINDOW_CENTER = ImVec2(ImGui::GetWindowWidth() / 2.f, ImGui::GetWindowHeight() / 2.f);
    for (const auto &button : list_buttons) {
        const auto POS_BUTTON = get_pos_button(button);
        if (POS_BUTTON.x) {
            ImGui::SetCursorPos(ImVec2(WINDOW_CENTER.x + (POS_BUTTON.x * SCALE.x), WINDOW_CENTER.y - (POS_BUTTON.y * SCALE.y)));
            remapper_button(gui, host, button, SCALE.x);
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - (68.f * SCALE.y));
    ImGui::TextColored(GUI_COLOR_TEXT_MENUBAR, "%-16s", "GUI");
    ImGui::Text("%-16s    %-16s", "Toggle Touch", "T");
    ImGui::Text("%-16s    %-16s", "Toggle GUI visibility", "G");

    ImGui::End();
}

} // namespace gui
