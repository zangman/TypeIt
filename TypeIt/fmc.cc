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

#include "fmc.h"
#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMUtilities.h>

FMC::FMC() : is_fmc_enabled_{false} {}

FMC::~FMC() {}

void FMC::SetupCommands() {
  XPLMCommandRef fmc_ref =
      XPLMCreateCommand(fmc_cmd_name_.c_str(), fmc_cmd_desc_.c_str());

  XPLMRegisterCommandHandler(fmc_ref, C_FMCCmdHandler, 1, this);
}

void FMC::SetupWidgets() {
  fmc_widget_ = XPCreateWidget(0, 370, 100, 350, 0, "fmc", 1, nullptr,
                               xpWidgetClass_Caption);

  XPSetWidgetProperty(fmc_widget_, xpProperty_CaptionLit, 1);
}

int FMC::CommandHandler(XPLMCommandRef in_command, XPLMCommandPhase in_phase) {
  if (in_phase != xplm_CommandEnd) {
    return 0;
  }

  XPLMSpeakString("FMC input enabled.");
  if (!is_fmc_enabled_) {
    XPShowWidget(fmc_widget_);
    XPLMRegisterKeySniffer(C_FMCKeySniffer, 0, this);
    is_fmc_enabled_ = true;
  } else {
    XPLMSpeakString("FMC input disabled.");
    XPHideWidget(fmc_widget_);
    XPLMUnregisterKeySniffer(C_FMCKeySniffer, 0, this);
    is_fmc_enabled_ = false;
  }

  return 0;
}

int FMC::CommandKeySniffer(char in_char, XPLMKeyFlags in_flags,
                           char in_virtual_key) {
  unsigned char in_vkey = static_cast<unsigned char>(in_virtual_key);
  if (in_flags == (xplm_ShiftFlag | xplm_DownFlag) ||
      in_flags == (xplm_ShiftFlag | xplm_UpFlag)) {
    auto search = fmc_shift_keys_.find(in_vkey);
    // Is element found?
    if (search != fmc_shift_keys_.end()) {
      std::string full_path{fmc_dataref_ + search->second};
      XPLMDataRef ref = XPLMFindDataRef(full_path.c_str());

      if (in_flags == (xplm_ShiftFlag | xplm_DownFlag)) {
        XPLMSetDatad(ref, 1.0);
      } else if (in_flags == (xplm_ShiftFlag | xplm_UpFlag)) {
        XPLMSetDatad(ref, 0.0);
      }
      return 0;
    } else {
      return 1;
    }
  } else if (in_flags == xplm_DownFlag || in_flags == xplm_UpFlag) {
    auto search = fmc_keys_.find(in_vkey);

    // Is the element found?
    if (search != fmc_keys_.end()) {
      std::string full_path{fmc_dataref_ + search->second};
      XPLMDataRef ref = XPLMFindDataRef(full_path.c_str());

      if (in_flags == xplm_DownFlag) {
        XPLMSetDatad(ref, 1.0);
      } else if (in_flags == xplm_UpFlag) {
        XPLMSetDatad(ref, 0.0);
      }
    }
    return 0;
  }

  return 1;
}

extern "C" int C_FMCCmdHandler(XPLMCommandRef in_command,
                               XPLMCommandPhase in_phase, void* in_refcon) {
  FMC* fmc = static_cast<FMC*>(in_refcon);

  return fmc->CommandHandler(in_command, in_phase);
}

extern "C" int C_FMCKeySniffer(char in_char, XPLMKeyFlags in_flags,
                               char in_virtual_key, void* in_refcon) {
  FMC* fmc = static_cast<FMC*>(in_refcon);
  return fmc->CommandKeySniffer(in_char, in_flags, in_virtual_key);
}