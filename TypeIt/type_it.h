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
#include <memory>
#include <string>
#include "fmc.h"
#include "radio.h"

class TypeIt {
 public:
  // Singleton.
  static TypeIt& GetInstance();

  // Does the initialization for all member variables for all objects.
  void Init(char* out_name, char* out_sig, char* out_desc);

  TypeIt(TypeIt const&) = delete;
  void operator=(TypeIt const&) = delete;

 private:
  const std::string name_{"TypeItPlugin"};
  const std::string sig_{"zang.TypeItPlugin"};
  const std::string desc_{"Plugin that allows typing into Radio and FMC."};

  std::unique_ptr<Radio> radio_;
  std::unique_ptr<FMC> fmc_;

  // Set up the commands for all objects.
  void SetupCommands();

  // Set up the widgets (if any) for all objects.
  void SetupWidgets();

  TypeIt();
  ~TypeIt();
};
