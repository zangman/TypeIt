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

#include <Widgets/XPStandardWidgets.h>
#include <Widgets/XPWidgets.h>
#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMUtilities.h>
#include <memory>
#include <string>
#include "type_it.h"

PLUGIN_API int XPluginStart(char* out_name, char* out_sig, char* out_desc) {
  // Instantiate and create a single instance of the class.
  TypeIt& type_it{TypeIt::GetInstance()};
  type_it.Init(out_name, out_sig, out_desc);
  return true;
}

PLUGIN_API void XPluginStop(void) {}

PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg,
                                      void* inParam) {}