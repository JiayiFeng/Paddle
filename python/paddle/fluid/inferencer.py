#   Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import core
import framework
import executor
import io
__all__ = ['Inferencer', ]


class Inferencer(object):
    def __init__(self, network_func, param_path=None, place=None):
        # 1. we need to generate a framework.Program by calling
        # network_func. Reference: fluid.program_guard in test_word2vec.py

        # 2. move the default_main_program to self.program.

        # 3. run the default_startup program.

        # 4. load params from param_path into scope
        self.scope = core.Scope()
        self.place = place
        self.startup_program = framework.Program()
        # TODO: generate the startup_program with network_func

        exe = executor.Executor(place)
        exe.run(self.startup_program, scope=self.scope)

        if param_path:
            # load params from param_path into scope
            io.load_persistables(exe, dirname=param_path)

    def infer(self, inputs):
        # run self.program
        pass
