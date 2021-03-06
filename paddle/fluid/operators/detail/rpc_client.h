// Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>

#include "paddle/fluid/framework/data_type.h"
#include "paddle/fluid/framework/lod_tensor.h"
#include "paddle/fluid/framework/scope.h"

namespace paddle {
namespace operators {
namespace detail {

class RPCClient {
 public:
  RPCClient() {}
  virtual ~RPCClient() {}
  virtual bool AsyncSendVar(const std::string& ep,
                            const platform::DeviceContext& ctx,
                            const framework::Scope& scope,
                            const std::string& var_name,
                            int64_t time_out = rpc_time_out) = 0;

  virtual bool AsyncGetVar(const std::string& ep,
                           const platform::DeviceContext& ctx,
                           const framework::Scope& scope,
                           const std::string& var_name,
                           int64_t time_out = rpc_time_out) = 0;

  virtual bool AsyncPrefetchVar(const std::string& ep,
                                const platform::DeviceContext& ctx,
                                const framework::Scope& scope,
                                const std::string& in_var_name,
                                const std::string& out_var_name,
                                int64_t time_out = rpc_time_out) = 0;

  virtual void AsyncSendBatchBarrier(const std::string& ep,
                                     int64_t time_out = rpc_time_out) = 0;

  virtual void AsyncSendFetchBarrier(const std::string& ep,
                                     int64_t time_out = rpc_time_out) = 0;

  virtual void Wait() = 0;

  static constexpr int64_t rpc_time_out = 120 * 1000;

  template <typename T>
  static RPCClient* GetInstance() {
    std::call_once(init_flag_, &RPCClient::Init<T>);
    return rpc_client_.get();
  }

  // Init is called by GetInstance.
  template <typename T>
  static void Init() {
    if (rpc_client_.get() == nullptr) {
      rpc_client_.reset(new T());
      rpc_client_->InitImpl();
    }
  }

 protected:
  virtual void InitImpl() {}

 private:
  static std::once_flag init_flag_;
  static std::unique_ptr<RPCClient> rpc_client_;
};
}  // namespace detail
}  // namespace operators
}  // namespace paddle
