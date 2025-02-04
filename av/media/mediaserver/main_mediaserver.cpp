/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#define LOG_TAG "mediaserver"
//#define LOG_NDEBUG 0

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

// from LOCAL_C_INCLUDES
#include "AudioFlinger.h"
#include "CameraService.h"
#include "DisplayService.h"
#include "MediaPlayerService.h"
#include "AudioPolicyService.h"

using namespace android;

//extern "C" int cedarx_hardware_init(int mode);
//extern "C" int cedarx_hardware_exit(int mode);

int main(int argc, char** argv)
{
    signal(SIGPIPE, SIG_IGN);

	//获取一个ProcessState实例
    sp<ProcessState> proc(ProcessState::self());

	//得到一个ServiceManager对象
    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: %p", sm.get());
    AudioFlinger::instantiate();
    MediaPlayerService::instantiate();
    //cedarx_hardware_init(1);
    //cedarx_hardware_exit(1);
    ALOGI("cedarx_hardware_init");
    CameraService::instantiate();
	DisplayService::instantiate();
    AudioPolicyService::instantiate();
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

}
