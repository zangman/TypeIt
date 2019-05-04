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

#include "type_it.h"
#include "util.h"

TypeIt& TypeIt::GetInstance() {
  static TypeIt instance;
  return instance;
}

void TypeIt::Init(char* out_name, char* out_sig, char* out_desc) {
  name_.copy(out_name, name_.size(), 0);
  sig_.copy(out_sig, sig_.size(), 0);
  desc_.copy(out_desc, desc_.size(), 0);

  radio_ = std::make_unique<Radio>();
  fmc_ = std::make_unique<FMC>();

  SetupCommands();
  SetupWidgets();
}

void TypeIt::SetupCommands() {
  radio_->SetupCommands();
  fmc_->SetupCommands();
}

void TypeIt::SetupWidgets() { radio_->SetupWidgets(); }

TypeIt::TypeIt() {}

TypeIt::~TypeIt() {}
