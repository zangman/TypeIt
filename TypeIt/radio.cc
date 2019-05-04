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

#include "radio.h"
#include <Widgets/XPStandardWidgets.h>
#include <Widgets/XPWidgets.h>
#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMUtilities.h>
#include "util.h"

Radio::Radio() {}

Radio::~Radio() {}

void Radio::SetupCommands() {
  com1_pair_ = std::make_unique<RadioPair>(this, &com1_data_);
  com2_pair_ = std::make_unique<RadioPair>(this, &com2_data_);
  nav1_pair_ = std::make_unique<RadioPair>(this, &nav1_data_);
  nav2_pair_ = std::make_unique<RadioPair>(this, &nav2_data_);
  transp_pair_ = std::make_unique<RadioPair>(this, &transp_data_);

  XPLMCommandRef com1_ref =
      XPLMCreateCommand(com1_cmd_name_.c_str(), com1_cmd_desc_.c_str());
  XPLMCommandRef com2_ref =
      XPLMCreateCommand(com2_cmd_name_.c_str(), com2_cmd_desc_.c_str());
  XPLMCommandRef nav1_ref =
      XPLMCreateCommand(nav1_cmd_name_.c_str(), nav1_cmd_desc_.c_str());
  XPLMCommandRef nav2_ref =
      XPLMCreateCommand(nav2_cmd_name_.c_str(), nav2_cmd_desc_.c_str());

  XPLMCommandRef transp_ref =
      XPLMCreateCommand(transp_cmd_name_.c_str(), transp_cmd_desc_.c_str());

  XPLMRegisterCommandHandler(com1_ref, C_ComCmdHandler, 1, com1_pair_.get());
  XPLMRegisterCommandHandler(com2_ref, C_ComCmdHandler, 1, com2_pair_.get());
  XPLMRegisterCommandHandler(nav1_ref, C_ComCmdHandler, 1, nav1_pair_.get());
  XPLMRegisterCommandHandler(nav2_ref, C_ComCmdHandler, 1, nav2_pair_.get());
  XPLMRegisterCommandHandler(transp_ref, C_ComCmdHandler, 1,
                             transp_pair_.get());
}

void Radio::SetupWidgets() {
  text_widget_ = XPCreateWidget(100, 370, 250, 350, 0, "", 1, nullptr,
                                xpWidgetClass_TextField);

  label_widget_ = XPCreateWidget(0, 370, 100, 350, 0, "label", 1, nullptr,
                                 xpWidgetClass_Caption);

  XPSetWidgetProperty(text_widget_, xpProperty_TextFieldType,
                      xpTextTranslucent);
  XPSetWidgetProperty(label_widget_, xpProperty_CaptionLit, 1);
  XPAddWidgetCallback(text_widget_, C_ComKeyHandler);
}

int Radio::CommandHandler(
    XPLMCommandRef in_command, XPLMCommandPhase in_phase,
    std::pair<Radio*, const std::tuple<std::string, std::string>*>*
        radio_pair) {
  if (in_phase != xplm_CommandEnd) {
    return 0;
  }

  XPSetWidgetProperty(text_widget_, xpProperty_Object,
                      reinterpret_cast<intptr_t>(radio_pair));
  const DataTup* tup = std::get<1>(*radio_pair);
  XPSetWidgetDescriptor(label_widget_, std::get<0>(*tup).c_str());

  XPShowWidget(text_widget_);
  XPShowWidget(label_widget_);
  XPSetKeyboardFocus(text_widget_);
  return 0;
}

int Radio::CommandKeyHandler(XPWidgetMessage in_message, XPWidgetID in_widget,
                             intptr_t in_param1, intptr_t in_param2) {
  if (in_message == xpMsg_KeyPress) {
    XPKeyState_t* key_state = reinterpret_cast<XPKeyState_t*>(in_param1);

    if (key_state->flags == xplm_DownFlag) {
      if (key_state->vkey == XPLM_VK_ENTER ||
          key_state->vkey == XPLM_VK_RETURN ||
          key_state->vkey == XPLM_VK_NUMPAD_ENT ||
          key_state->key == XPLM_KEY_RETURN) {
        char input_text[40];

        XPGetWidgetDescriptor(text_widget_, input_text, 40);

        RadioPair* radio_pair = reinterpret_cast<RadioPair*>(
            XPGetWidgetProperty(in_widget, xpProperty_Object, nullptr));

        const DataTup* tup = std::get<1>(*radio_pair);

        auto name = std::get<0>(*tup);
        auto path = std::get<1>(*tup);

        int freq = -1;

        if (name == "transponder") {
          freq = ConvertTransponderCode(input_text);
        } else {
          freq = ConvertRadioFreq(input_text, name);
        }

        // If invalid frequency.
        if (freq < 0) {
          XPLMSpeakString("Invalid frequency");
        } else {
          XPLMDataRef radio_ref = XPLMFindDataRef(path.c_str());
          XPLMSetDatai(radio_ref, freq);
          std::string message{"Updating " + name + ": " + input_text};
          XPLMSpeakString(message.c_str());
        }

        Reset();
      } else if (key_state->key == XPLM_KEY_ESCAPE ||
                 key_state->vkey == XPLM_VK_ESCAPE) {
        Reset();
      }
    }

  } else if (in_message == xpMsg_TextFieldChanged) {
    XPLMSpeakString("Text changed");
  }

  return 0;
}

int Radio::ConvertRadioFreq(std::string freq_str, std::string name) {
  // This needs to be 6 digit number.
  int final_freq_val = 0;

  // If it is COM, it can have a max of 7 characters or 6 significant digits.
  if (name == std::get<0>(com1_data_) || name == std::get<0>(com2_data_)) {
    if (freq_str.length() < 3 || freq_str.length() > 7) {
      // Invalid format (max 7 characters including the period.)
      return -1;
    }

    float freq_val = 0.0f;

    try {
      freq_val = std::stof(freq_str);
    } catch (std::invalid_argument e) {
      l("Invalid freq value: " + freq_str);
      return -1;
    }

    // COM freq ranges from 118 to 137.
    if (freq_val < 118.0f || freq_val >= 137.0f) {
      l("Invalid COM freq value: " + freq_str);
      return -1;
    }

    final_freq_val = static_cast<int>(freq_val * 1000);
  }
  // If it is NAV, it can have a max of 6 characters or 5 significant digits.
  else {
    if (freq_str.length() < 3 || freq_str.length() > 6) {
      // Invalid format (max 7 characters including the period.)
      return -1;
    }

    float freq_val = 0.0f;

    try {
      freq_val = std::stof(freq_str);
    } catch (std::invalid_argument e) {
      l("Invalid freq value: " + freq_str);
      return -1;
    }

    // COM freq ranges from 108 to 118.
    if (freq_val < 108.0f || freq_val >= 118.0f) {
      l("Invalid NAV freq value: " + freq_str);
      return -1;
    }

    final_freq_val = static_cast<int>(freq_val * 100);
  }

  return final_freq_val;
}

int Radio::ConvertTransponderCode(std::string freq_str) {
  int freq_val = 0;
  try {
    freq_val = std::stoi(freq_str);
  } catch (std::invalid_argument e) {
    l("Invalid transponder code: " + freq_str);
    return -1;
  }

  // Transponder value can never be more than 7777.
  if (freq_val > 7777) {
    l("Invalid transponder code: " + freq_str);
    return -1;
  }

  return freq_val;
}

void Radio::Reset() {
  XPSetWidgetDescriptor(text_widget_, "");
  XPHideWidget(text_widget_);
  XPHideWidget(label_widget_);
}

extern "C" int C_ComCmdHandler(XPLMCommandRef in_command,
                               XPLMCommandPhase in_phase, void* in_refcon) {
  RadioPair* radio_pair = static_cast<RadioPair*>(in_refcon);
  Radio* radio = std::get<0>(*radio_pair);
  return radio->CommandHandler(in_command, in_phase, radio_pair);
}

extern "C" int C_ComKeyHandler(XPWidgetMessage in_message, XPWidgetID in_widget,
                               intptr_t in_param1, intptr_t in_param2) {
  if (in_message == xpMsg_KeyPress) {
    RadioPair* radio_pair = reinterpret_cast<RadioPair*>(
        XPGetWidgetProperty(in_widget, xpProperty_Object, nullptr));

    Radio* radio = std::get<0>(*radio_pair);

    return radio->CommandKeyHandler(in_message, in_widget, in_param1,
                                    in_param2);
  }
  return 0;
}