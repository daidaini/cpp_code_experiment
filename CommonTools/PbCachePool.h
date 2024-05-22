/*
 * @Descripttion: 使用boost::pool, 封装一个线程安全的内存池
 * @Author: yubo
 * @Date: 2022-10-12 17:12:58
 * @LastEditTime: 2022-10-13 09:41:28
 */
#pragma once

#include <boost/pool/pool.hpp>

#include <mutex>

namespace pobo
{
    template <int ChunkBlockSize>
    class CachePool final
    {
        using CacheUnitType = char[ChunkBlockSize];

    public:
        CachePool()
            : m_Pool(ChunkBlockSize)
        {
        }

        ~CachePool() = default;

        void *Allocate(size_t size)
        {
            if (size > ChunkBlockSize) // 申请内存超长，退化为默认的new分配
            {
                return malloc(size);
            }

            std::lock_guard<std::mutex> guard(m_Mtx);
            return m_Pool.malloc();
        }

        void Deallocate(void *cache, int size)
        {
            if (size > ChunkBlockSize) // 申请内存超长，退化为默认的new分配
            {
                if (cache != nullptr)
                {
                    free(cache);
                    cache = nullptr;
                }
                return;
            }

            std::lock_guard<std::mutex> guard(m_Mtx);
            m_Pool.free(cache);
        }

    private:
        boost::pool<> m_Pool;

        std::mutex m_Mtx;
    };
}