############################################################################
#
# Copyright (C) 2022-2024 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################

include $(APPDIR)/Make.defs

ifneq ($(CONFIG_SC_SET_MODEL_API),)
CSRCS += set_model_api.c
ifneq ($(CONFIG_SC_SET_MODEL_TOOL),)
PROGNAME  = ${CONFIG_SC_SET_MODEL_PROGNAME}
PRIORITY  = ${CONFIG_SC_SET_MODEL_PRIORITY}
STACKSIZE = ${CONFIG_SC_SET_MODEL_STACKSIZE}
MODULE    = ${CONFIG_SC_SET_MODEL_TOOL}
MAINSRC   = set_model.c
endif
endif

include $(APPDIR)/Application.mk
