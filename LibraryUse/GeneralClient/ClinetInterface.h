#pragma once

#define HD_API extern "C" // __attribute__((visibility("default")))

#include "ClientDataDefine.h"

/**
 * @brief Create a Client object
 *
 * @param ipAddr 服务地址，格式  ip:port
 * @param cb 消息回调函数
 * @return client句柄
 */
HD_API void *HDCreateClient(const char *ipAddr, OnConnectionFuncType connCb, OnMsgFuncType cb);

/**
 * @brief 释放client实例
 *
 * @param client
 * @return
 */
HD_API void HDDeleteCleint(void *client);

/**
 * @brief 发送请求消息
 *
 * @param msg
 * @param msglen
 */
HD_API void HDSendMsg(void *client, const char *msg, size_t msglen);