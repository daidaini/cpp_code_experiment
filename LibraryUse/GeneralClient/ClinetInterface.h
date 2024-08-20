#pragma once

/** 通用的网络消息交互库
 *  异步，单独线程处理网络消息，底层使用boost.asio
 *  C++库，封装为C接口调用，可动态加载
 *  使用：
 *  Create -> SendMsg .... -> Delete
 */

#define HD_API extern "C" // __attribute__((visibility("default")))

#include "ClientDataDefine.h"

/**
 * @brief Create a Client object
 *
 * @param ipAddr 服务地址，格式  ip:port
 * @param msgCb 消息回调函数
 * @return client句柄
 */
HD_API void *HDCreateClient(const char *ipAddr, OnConnectionFuncType connCb, OnMsgFuncType msgCb);

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