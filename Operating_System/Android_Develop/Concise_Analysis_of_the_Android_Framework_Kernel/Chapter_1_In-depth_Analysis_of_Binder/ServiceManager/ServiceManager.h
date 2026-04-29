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

#pragma once

#include <android/os/BnServiceManager.h>
#include <android/os/IClientCallback.h>
#include <android/os/IServiceCallback.h>

#include "Access.h"

namespace android {

using os::IClientCallback;
using os::IServiceCallback;

// 类定义，ServiceManager 同时继承了 os::BnServiceManager 和 IBinder::DeathRecipient
class ServiceManager : public os::BnServiceManager, public IBinder::DeathRecipient
{
    // 对外提供的服务管理能力
public:
    // 构造和析构，创建 ServiceManager 对象时需要传入一个 Access 对象的独占智能指针
    ServiceManager(std::unique_ptr<Access>&& access);
    ~ServiceManager();

    // getService will try to start any services it cannot find
    // 添加服务
    binder::Status getService(const std::string& name, sp<IBinder>* outBinder) override;
    binder::Status checkService(const std::string& name, sp<IBinder>* outBinder) override;
    // 服务注册
    binder::Status addService(const std::string& name, const sp<IBinder>& binder,
        bool allowIsolated, int32_t dumpPriority) override;
    // 列出服务
    binder::Status listServices(int32_t dumpPriority, std::vector<std::string>* outList) override;
    // 注册/取消服务通知
    binder::Status registerForNotifications(const std::string& name,
                                            const sp<IServiceCallback>& callback) override;
    binder::Status unregisterForNotifications(const std::string& name,
                                              const sp<IServiceCallback>& callback) override;
    // 查询是否声明
    binder::Status isDeclared(const std::string& name, bool* outReturn) override;
    // 注册客户端回调
    binder::Status registerClientCallback(const std::string& name, const sp<IBinder>& service,
                                          const sp<IClientCallback>& cb) override;
    // 反注册服务
    binder::Status tryUnregisterService(const std::string& name, const sp<IBinder>& binder) override;
    // 死亡通知
    void binderDied(const wp<IBinder>& who) override;
    // 周期性处理客户端状态
    void handleClientCallbacks();

protected:
    // getService 查不到服务时，尝试启动该服务
    virtual void tryStartService(const std::string& name);

private:
    // ServiceManager 记录的一条服务信息
    struct Service
    {
        sp<IBinder> binder; // not null
        bool allowIsolated;
        int32_t dumpPriority;
        bool hasClients = false; // notifications sent on true -> false.
        bool guaranteeClient = false; // forces the client check to true
        pid_t debugPid = 0; // the process in which this service runs

        // the number of clients of the service, including servicemanager itself
        ssize_t getNodeStrongRefCount();
    };
    // ServiceCallbackMap  服务名到服务回调对象的映射
    using ServiceCallbackMap = std::map<std::string, std::vector<sp<IServiceCallback>>>;
    // ClientCallbackMap  服务名到客户端回调对象的映射    
    using ClientCallbackMap = std::map<std::string, std::vector<sp<IClientCallback>>>;
    // ServiceMap  服务名到服务对象的映射
    using ServiceMap = std::map<std::string, Service>;

    // 辅助函数
    // removes a callback from mNameToRegistrationCallback, removing it if the vector is empty
    // this updates iterator to the next location
    void removeRegistrationCallback(const wp<IBinder>& who,
                        ServiceCallbackMap::iterator* it,
                        bool* found);
    ssize_t handleServiceClientCallback(const std::string& serviceName, bool isCalledOnInterval);
     // Also updates mHasClients (of what the last callback was)
    void sendClientCallbackNotifications(const std::string& serviceName, bool hasClients);
    // removes a callback from mNameToClientCallback, deleting the entry if the vector is empty
    // this updates the iterator to the next location
    void removeClientCallback(const wp<IBinder>& who, ClientCallbackMap::iterator* it);

    sp<IBinder> tryGetService(const std::string& name, bool startIfNotFound);
    // 成员变量
    ServiceMap mNameToService;
    ServiceCallbackMap mNameToRegistrationCallback;
    ClientCallbackMap mNameToClientCallback;
    // 权限控制
    std::unique_ptr<Access> mAccess;
};

}  // namespace android
