/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android-base/logging.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/Status.h>
#include <sys/timerfd.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

#include "Access.h"
#include "ServiceManager.h"

using ::android::Access;
using ::android::sp;
using ::android::Looper;
using ::android::LooperCallback;
using ::android::ProcessState;
using ::android::IPCThreadState;
using ::android::ProcessState;
using ::android::ServiceManager;
using ::android::os::IServiceManager;
using ::android::sp;

// 处理 Binder_fd 上来的事件
class BinderCallback : public LooperCallback
{
public:
    static sp<BinderCallback> setupTo(const sp<Looper>& looper) {
        sp<BinderCallback> cb = new BinderCallback;

        int binder_fd = -1;
        // 从 binder 拿到一个 binder_fd
        IPCThreadState::self()->setupPolling(&binder_fd);
        LOG_ALWAYS_FATAL_IF(binder_fd < 0, "Failed to setupPolling: %d", binder_fd);

        // Flush after setupPolling(), to make sure the binder driver
        // knows about this thread handling commands.
        // 把前面和 Binder 驱动相关的命令刷新下去，去确保驱动知道当前线程要处理的 Binder 命令
        IPCThreadState::self()->flushCommands();
        // 把这个 fd 挂钩到 looper 上，监听输入事件，事件发生时回调 cb 的 handleEvent 方法
        int ret = looper->addFd(binder_fd,
                                Looper::POLL_CALLBACK,
                                Looper::EVENT_INPUT,
                                cb,
                                nullptr /*data*/);
        LOG_ALWAYS_FATAL_IF(ret != 1, "Failed to add binder FD to Looper");

        return cb;
    }

    int handleEvent(int /* fd */, int /* events */, void* /* data */) override {
        IPCThreadState::self()->handlePolledCommands();
        return 1;  // Continue receiving callbacks.
    }
};

// LooperCallback for IClientCallback
// 周期性检查服务有没有客户端
class ClientCallbackCallback : public LooperCallback
{
public:
    static sp<ClientCallbackCallback> setupTo(const sp<Looper>& looper, const sp<ServiceManager>& manager) {
        sp<ClientCallbackCallback> cb = new ClientCallbackCallback(manager);
        // 创建 timer_fd 
        int fdTimer = timerfd_create(CLOCK_MONOTONIC, 0 /*flags*/);
        LOG_ALWAYS_FATAL_IF(fdTimer < 0, "Failed to timerfd_create: fd: %d err: %d", fdTimer, errno);

        itimerspec timespec {
            .it_interval = {
                .tv_sec = 5,
                .tv_nsec = 0,
            },
            .it_value = {
                .tv_sec = 5,
                .tv_nsec = 0,
            },
        };
        // 5s 触发一次
        int timeRes = timerfd_settime(fdTimer, 0 /*flags*/, &timespec, nullptr);
        LOG_ALWAYS_FATAL_IF(timeRes < 0, "Failed to timerfd_settime: res: %d err: %d", timeRes, errno);
        // 添加 fd 到 looper，定时提醒 ServiceManager 去检查客户端状态
        int addRes = looper->addFd(fdTimer,
                                   Looper::POLL_CALLBACK,
                                   Looper::EVENT_INPUT,
                                   cb,
                                   nullptr);
        LOG_ALWAYS_FATAL_IF(addRes != 1, "Failed to add client callback FD to Looper");

        return cb;
    }

    int handleEvent(int fd, int /*events*/, void* /*data*/) override {
        uint64_t expirations;
        int ret = read(fd, &expirations, sizeof(expirations));
        if (ret != sizeof(expirations)) {
            ALOGE("Read failed to callback FD: ret: %d err: %d", ret, errno);
        }
        // 每次调用就触发
        mManager->handleClientCallbacks();
        return 1;  // Continue receiving callbacks.
    }
private:
    ClientCallbackCallback(const sp<ServiceManager>& manager) : mManager(manager) {}
    sp<ServiceManager> mManager;
};

int main(int argc, char** argv)
{
    // 去顶 Binder 驱动路径，默认是 /dev/binder
    if (argc > 2) {
        LOG(FATAL) << "usage: " << argv[0] << " [binder driver]";
    }

    const char* driver = argc == 2 ? argv[1] : "/dev/binder";

    // 强引用智能指针，调用 ProcessState 类的静态成员函数，初始化当前进程的 Binder 状态，并打开指定的 Binder 驱动设备
    // 初始化 Binder 进程状态
    sp<ProcessState> ps = ProcessState::initWithDriver(driver);
    ps->setThreadPoolMaxThreadCount(0);
    ps->setCallRestriction(ProcessState::CallRestriction::FATAL_IF_NOT_ONEWAY);
    // 创建 ServiceManager 对象，把自己注册成第一个服务
    sp<ServiceManager> manager = new ServiceManager(std::make_unique<Access>());
    if (!manager->addService("manager", manager, false /*allowIsolated*/, IServiceManager::DUMP_FLAG_PRIORITY_DEFAULT).isOk()) {
        LOG(ERROR) << "Could not self register servicemanager";
    }
    // 设置上下文对象，并成为上下文管理者
    IPCThreadState::self()->setTheContextObject(manager);
    ps->becomeContextManager(nullptr, nullptr);
    // 准备 Looper，并进入死循环
    sp<Looper> looper = Looper::prepare(false /*allowNonCallbacks*/);

    BinderCallback::setupTo(looper);
    ClientCallbackCallback::setupTo(looper, manager);

    while(true) {
        looper->pollAll(-1);
    }

    // should not be reached
    return EXIT_FAILURE;
}
