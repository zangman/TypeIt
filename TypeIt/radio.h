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
#include <Widgets/XPWidgets.h>
#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMUtilities.h>
#include <string>
#include <tuple>
class Radio {
 public:
  Radio();
  ~Radio();

  void SetupCommands();
  void SetupWidgets();

  // Called when the command is triggered.
  int CommandHandler(
      XPLMCommandRef in_command, XPLMCommandPhase in_phase,
      std::pair<Radio*, const std::tuple<std::string, std::string>*>*
          radio_pair);

  // Called by the text widget when it is not hidden.
  int CommandKeyHandler(XPWidgetMessage in_message, XPWidgetID in_widget,
                        intptr_t in_param1, intptr_t in_param2);

  // Converts radio frequency to an int from string for the dataref.
  // Possible COM inputs and values:
  // 1. 118.    ==> 118000
  // 2. 118.1   ==> 118100
  // 3. 118.15  ==> 118150
  // 4. 118.125 ==> 118125
  // NAV is same as above, but only 2 digits after decimal.
  int ConvertRadioFreq(std::string freq_str, std::string name);

  // Converts transponder code to an int from string for the dataref.
  int ConvertTransponderCode(std::string transp_str);

  // Reset the widget.
  void Reset();

 private:
  // Data pairs that are passed to the X-Plane SDK either through void* inRefcon
  // or through the widget property.
  std::unique_ptr<
      std::pair<Radio*, const std::tuple<std::string, std::string>*>>
      com1_pair_;
  std::unique_ptr<
      std::pair<Radio*, const std::tuple<std::string, std::string>*>>
      com2_pair_;
  std::unique_ptr<
      std::pair<Radio*, const std::tuple<std::string, std::string>*>>
      nav1_pair_;
  std::unique_ptr<
      std::pair<Radio*, const std::tuple<std::string, std::string>*>>
      nav2_pair_;
  std::unique_ptr<
      std::pair<Radio*, const std::tuple<std::string, std::string>*>>
      transp_pair_;

  const std::string com1_cmd_name_{"TypeIt/set_com1_standby"};
  const std::string com1_cmd_desc_{"Set COM1 Standby Frequency."};
  const std::string com2_cmd_name_{"TypeIt/set_com2_standby"};
  const std::string com2_cmd_desc_{"Set COM2 Standby Frequency."};

  const std::string nav1_cmd_name_{"TypeIt/set_nav1_standby"};
  const std::string nav1_cmd_desc_{"Set NAV1 Standby Frequency."};
  const std::string nav2_cmd_name_{"TypeIt/set_nav2_standby"};
  const std::string nav2_cmd_desc_{"Set NAV2 Standby Frequency."};

  const std::string transp_cmd_name_{"TypeIt/set_transponder_code"};
  const std::string transp_cmd_desc_{"Set Transponder/Squawk code."};

  const std::tuple<std::string, std::string> com1_data_{
      "com1", "sim/cockpit2/radios/actuators/com1_standby_frequency_hz_833"};
  const std::tuple<std::string, std::string> com2_data_{
      "com2", "sim/cockpit2/radios/actuators/com2_standby_frequency_hz_833"};
  const std::tuple<std::string, std::string> nav1_data_{
      "nav1", "sim/cockpit2/radios/actuators/nav1_standby_frequency_hz"};
  const std::tuple<std::string, std::string> nav2_data_{
      "nav2", "sim/cockpit2/radios/actuators/nav2_standby_frequency_hz"};

  const std::tuple<std::string, std::string> transp_data_{
      "transponder", "sim/cockpit2/radios/actuators/transponder_code"};

  XPWidgetID text_widget_;
  XPWidgetID label_widget_;
};

using DataTup = std::tuple<std::string, std::string>;
using RadioPair = std::pair<Radio*, const DataTup*>;

// This is the callback function for the command handler which is passed to the
// X-Plane SDK.
extern "C" int C_ComCmdHandler(XPLMCommandRef in_command,
                               XPLMCommandPhase in_phase, void* in_refcon);

// This is the callback function for the key handler which is passed to the
// X-Plane SDK.
extern "C" int C_ComKeyHandler(XPWidgetMessage in_message, XPWidgetID in_widget,
                               intptr_t in_param1, intptr_t in_param2);
