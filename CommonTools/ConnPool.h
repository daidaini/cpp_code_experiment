#pragma once

/*从WindowsConnPool迁移而来
 * Tips：
 * 存储连接的容器采用stack，后进先出，在于：
 * 尽量使用最近使用过的连接，保活的连接与并发需求的连接数一致，多余的连接容易失效并被释放
 * 缺点：用户请求的频率并不是一致的，某个时间段请求增多（例如行情波动大，下单频次增加），需要的连接数增加，请求到达时，发现没有连接可用,创建连接，
 *			而创建连接是比较耗时的，从而增加整个请求的延时
 * 此外：
 *  该连接池一般适用于跟柜台需要同步请求的时候（同步请求则意味着有可能某个连接会因为某个耗时的请求长时间被占用）
 */

#include <ctime>
#include <vector>
#include <exception>
#include <mutex>

#include "STD.h"

template <class _Tx>
class CConnPool
{
protected:
    enum
    {
        MAX_CONN_NUM = 8192
    };

public:
    /**构造函数
     */
    CConnPool()
    {
        m_Conns.reserve(MAX_CONN_NUM);
        m_LastCheckTime = time(NULL);
    }

    /**析构函数
     */
    virtual ~CConnPool()
    {
        this->ReleasePool();
    }

    /**释放连接池
     */
    void ReleasePool()
    {
        std::lock_guard<std::mutex> guard(m_Mtx);
        for (auto &conn : m_Conns)
        {
            STD::Free(conn);
        }
    }

    /**返回一个连接,如果连接池为空，则创建一个新连接，并返回；如果创建失败，则返回nullptr。
     *@return 一个连接，可能为nullptr，调用者需要进行判空处理
     */
    _Tx *GetConnect()
    {
        {
            std::lock_guard<std::mutex> guard(m_Mtx);
            if (!m_Conns.empty())
            {
                _Tx *p = m_Conns.back();
                m_Conns.pop_back();
                return p;
            }
        }

        // 创建连接不能加锁
        return CreateConn();
    }

    /**返回一个连接，并检测连接状态；删除不可用连接；尝试找到一个可用连接，如果找不到，则创建一个新连接，并返回；
     *创建失败，则返回nullptr
     *@return 一个连接，可能为nullptr，调用者需要进行判空处理
     */
    _Tx *GetConnectWithCheckConnectFlag()
    {
        CheckConnect();

        return GetConnect();
    }

    /**返还一个连接,放在末尾
     */
    void PutConnect(_Tx *&pconn)
    {
        if (pconn == nullptr)
            return;

        std::lock_guard<std::mutex> guard(m_Mtx);
        if (m_Conns.size() < MAX_CONN_NUM)
        {
            m_Conns.push_back(pconn);
        }
        else // 满了
        {

            STD::Free(pconn);
        }
    }

    /**释放一个连接
     */
    void ReleaseConnect(_Tx *&pconn)
    {
        if (pconn == nullptr)
            return;
        STD::Free(pconn);
    }

protected:
    _Tx *CreateConn()
    {
        // 没有连接，则需创建一个新连接
        try
        {
            _Tx *p = new _Tx();
            if (p->Create())
            {
                return p;
            }
            else
            {
                STD::Free(p);
                return nullptr;
            }
        }
        catch (const std::exception &e)
        {
            return nullptr;
        }
    }

    // 遍历ConnStack，把无效连接清空
    void CheckConnect()
    {
        std::lock_guard<std::mutex> guard(m_Mtx);

        // 每秒检查一次
        time_t now = time(NULL);
        if (now - m_LastCheckTime < 1)
        {
            return;
        }

        size_t i = 0;
        while (i < m_Conns.size())
        {
            if (!m_Conns[i]->GetSocketValid())
            {
                STD::Free(m_Conns[i]);

                // 这一个与最后一个交换，并删除最后一个（该元素就是最后一个，算法也成立）
                m_Conns[i] = m_Conns.back();
                m_Conns.pop_back();

                // 这种情况i不需要递增，因为这个元素已经与最后一个交换了，最后一个是还没有检查的
            }
            else
            {
                i++;
            }
        }

        m_LastCheckTime = time(NULL);
    }

protected:
    std::vector<_Tx *> m_Conns;
    std::mutex m_Mtx;
    // 用于检测连接是否在线
    time_t m_LastCheckTime;
};