// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <Widgets/XPStandardWidgets.h>
#include <Widgets/XPWidgets.h>
#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMUtilities.h>
#include <string>
#include <unordered_map>
class FMC {
 public:
  FMC();
  ~FMC();

  void SetupCommands();
  void SetupWidgets();

  // Called when the command is triggered.
  int CommandHandler(XPLMCommandRef in_command, XPLMCommandPhase in_phase);

  // Called when the command is triggered and enabled.
  int CommandKeySniffer(char in_char, XPLMKeyFlags in_flags,
                        char in_virtual_key);

 private:
  const std::string fmc_cmd_name_{"TypeIt/start_fmc_mode"};
  const std::string fmc_cmd_desc_{"Toggle FMC Input mode."};

  XPWidgetID fmc_widget_;

  const std::string fmc_dataref_{"laminar/B738/buttons/fmc/capt/"};
  const std::unordered_map<int, std::string> fmc_keys_{
      {XPLM_VK_A, "a"},           {XPLM_VK_B, "b"},
      {XPLM_VK_C, "c"},           {XPLM_VK_D, "d"},
      {XPLM_VK_E, "e"},           {XPLM_VK_F, "f"},
      {XPLM_VK_G, "g"},           {XPLM_VK_H, "h"},
      {XPLM_VK_I, "i"},           {XPLM_VK_J, "j"},
      {XPLM_VK_K, "k"},           {XPLM_VK_L, "l"},
      {XPLM_VK_M, "m"},           {XPLM_VK_N, "n"},
      {XPLM_VK_O, "o"},           {XPLM_VK_P, "p"},
      {XPLM_VK_Q, "q"},           {XPLM_VK_R, "r"},
      {XPLM_VK_S, "s"},           {XPLM_VK_T, "t"},
      {XPLM_VK_U, "u"},           {XPLM_VK_V, "v"},
      {XPLM_VK_W, "w"},           {XPLM_VK_X, "x"},
      {XPLM_VK_Y, "y"},           {XPLM_VK_Z, "z"},
      {XPLM_VK_0, "0"},           {XPLM_VK_1, "1"},
      {XPLM_VK_2, "2"},           {XPLM_VK_3, "3"},
      {XPLM_VK_4, "4"},           {XPLM_VK_5, "5"},
      {XPLM_VK_6, "6"},           {XPLM_VK_7, "7"},
      {XPLM_VK_8, "8"},           {XPLM_VK_9, "9"},
      {XPLM_VK_NUMPAD0, "0"},     {XPLM_VK_NUMPAD1, "1"},
      {XPLM_VK_NUMPAD2, "2"},     {XPLM_VK_NUMPAD3, "3"},
      {XPLM_VK_NUMPAD4, "4"},     {XPLM_VK_NUMPAD5, "5"},
      {XPLM_VK_NUMPAD6, "6"},     {XPLM_VK_NUMPAD7, "7"},
      {XPLM_VK_NUMPAD8, "8"},     {XPLM_VK_NUMPAD9, "9"},
      {XPLM_VK_SPACE, "sp"},      {XPLM_VK_DECIMAL, "period"},
      {XPLM_VK_PERIOD, "period"}, {XPLM_VK_SLASH, "slash"},
      {XPLM_VK_DIVIDE, "slash"},  {XPLM_VK_SUBTRACT, "minus"},
      {XPLM_VK_MINUS, "minus"},   {XPLM_VK_BACK, "clr"},
      {XPLM_VK_DELETE, "clr"},    {XPLM_VK_PRIOR, "prev"},
      {XPLM_VK_NEXT, "next"},     {XPLM_VK_F1, "1L"},  // Left button 1
      {XPLM_VK_F2, "2L"},                              // Left button 2
      {XPLM_VK_F3, "3L"},                              // Left button 3
      {XPLM_VK_F4, "4L"},                              // Left button 4
      {XPLM_VK_F5, "5L"},                              // Left button 5
      {XPLM_VK_F6, "6L"},                              // Left button 6
      {XPLM_VK_F7, "1R"},                              // Right button 1
      {XPLM_VK_F8, "2R"},                              // Right button 2
      {XPLM_VK_F9, "3R"},                              // Right button 3
      {XPLM_VK_F10, "4R"},                             // Right button 4
      {XPLM_VK_F11, "5R"},                             // Right button 5
      {XPLM_VK_F12, "6R"}                              // Right button 6
  };

  // These are triggered with the shift key pressed.
  const std::unordered_map<int, std::string> fmc_shift_keys_{
      {XPLM_VK_C, "clb"},       // CLB
      {XPLM_VK_D, "dep_arr"},   // DEP ARR
      {XPLM_VK_S, "des"},       // DES
      {XPLM_VK_E, "exec"},      // EXEC
      {XPLM_VK_F, "fix"},       // FIX
      {XPLM_VK_H, "hold"},      // HOLD
      {XPLM_VK_I, "init_ref"},  // INIT REF
      {XPLM_VK_L, "legs"},      // LEGS
      {XPLM_VK_M, "menu"},      // MENU
      {XPLM_VK_N, "n1_limit"},  // N1 LIMIT
      {XPLM_VK_P, "prog"},      // PROG
      {XPLM_VK_R, "rte"},       // RTE
      {XPLM_VK_Z, "crz"},       // CRZ
      {XPLM_VK_DELETE, "del"},  // DEL
      {XPLM_VK_F1, "1R"},       // Right button 1
      {XPLM_VK_F2, "2R"},       // Right button 2
      {XPLM_VK_F3, "3R"},       // Right button 3
      {XPLM_VK_F4, "4R"},       // Right button 4
      {XPLM_VK_F5, "5R"},       // Right button 5
      {XPLM_VK_F6, "6R"}        // Right button 6
  };

  boolean is_fmc_enabled_;
};

// This is the callback function for the command handler which is passed to the
// X-Plane SDK.
extern "C" int C_FMCCmdHandler(XPLMCommandRef in_command,
                               XPLMCommandPhase in_phase, void* in_refcon);

// This is the callback function for the key sniffer which is passed to the
// X-Plane SDK.
extern "C" int C_FMCKeySniffer(char in_char, XPLMKeyFlags in_flags,
                               char in_virtual_key, void* in_refcon);