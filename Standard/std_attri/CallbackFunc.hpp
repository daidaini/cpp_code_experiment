#pragma once

#include "CommInc.h"

//使用函数指针作为回调
namespace CallbackFunc_0
{
    void OnResponse(const char *data, int len)
    {
        cout << "Calling OnResponse :";
        cout << std::string(data, len) << endl;
    }

    typedef void (*OnRspFuncPtr)(const char *data, int len);

    class Client
    {
    public:
        void RegisterCallback(OnRspFuncPtr func)
        {
            onRspFunc_ = func;
        }

        void Running()
        {
            for (int i = 0; i < 100; ++i)
            {
                this_thread::sleep_for(chrono::milliseconds(i));
                if (i % 20 == 5)
                {
                    std::string content = fmt::format("SendBack: {}", i);
                    onRspFunc_(content.data(), (int)content.length()); // error
                }
            }
        }

    private:
        OnRspFuncPtr onRspFunc_;
    };

    void Test()
    {
        Client cli;
        cli.RegisterCallback(OnResponse);
        cli.Running();
    }
}

//使用抽象类作为接口
namespace CallbackFunc_1
{
    //纯虚抽象类
    class CallbackInterface
    {
    public:
        virtual void OnResponse(const char *data, int len){}
        virtual ~CallbackInterface(){}
    };

    //被调用者继承抽象类，实现具体回调
    class Callback : public CallbackInterface
    {
    public:
        void OnResponse(const char *data, int len)
        {
            cout << "Calling OnResponse :";
            cout << std::string(data, len) << endl;
        }

        Callback() = default;
        ~Callback() = default;
    };

    //调用者代码
    class Client
    {
    public:
        void RegisterCallback(CallbackInterface* cb)
        {
            cb_ = cb;
        }

        //模拟使用回调函数
        void Running()
        {
            for (int i = 0; i < 100; ++i)
            {
                this_thread::sleep_for(chrono::milliseconds(i));
                if (i % 20 == 5)
                {
                    std::string content = fmt::format("SendBack: {}", i);
                    cb_->OnResponse(content.data(), (int)content.length()); // error
                }
            }
        }

    private:
        CallbackInterface *cb_;
    };

    void Test()
    {
        Client cli;
        CallbackInterface* cb = new Callback;
        cli.RegisterCallback(cb);
        cli.Running();
    }
}

//使用std::function，并且回调函数为类成员函数
namespace CallbackFunc_2
{
    class Callback
    {
    public:
        void OnResponse(const char *data, int len)
        {
            cout << "Calling OnResponse :";
            cout << std::string(data, len) << endl;
        }

        Callback() = default;
        ~Callback() = default;
    };
    typedef std::function<void(const char *, int)> OnRspFuncType;

    class Client
    {
    public:
        void RegisterCallback(OnRspFuncType func)
        {
            onRspFunc_ = std::move(func);
        }

        void Running()
        {
            for (int i = 0; i < 100; ++i)
            {
                this_thread::sleep_for(chrono::milliseconds(i));
                if (i % 20 == 5)
                {
                    std::string content = fmt::format("SendBack: {}", i);
                    onRspFunc_(content.data(), (int)content.length());
                }
            }
        }

    private:
        OnRspFuncType onRspFunc_;
    };

    void Test()
    {
        Callback cb;
        Client cli;
        cli.RegisterCallback(std::bind(&Callback::OnResponse, &cb, placeholders::_1, placeholders::_2));
        cli.Running();
    }
}

//使用函数对象来作为接口回调，是否可行
namespace CallbackFunc_3
{
    struct CallbackFunc
    {
        void operator()(const char *data, int len)
        {
            cout << "Function obj Calling OnResponse :";
            cout << std::string(data, len) << endl;
        }
    };

    class Client
    {
    public:
        void RegisterCallback(CallbackFunc func)
        {
            onRspFunc_ = func;
        }

        void Running()
        {
            for (int i = 0; i < 100; ++i)
            {
                this_thread::sleep_for(chrono::milliseconds(i));
                if (i % 20 == 5)
                {
                    std::string content = fmt::format("SendBack: {}", i);
                    onRspFunc_(content.data(), (int)content.length());
                }
            }
        }

    private:
        CallbackFunc onRspFunc_;
    };

    void Test()
    {
        CallbackFunc cb;
        Client cli;
        cli.RegisterCallback(cb);
        cli.Running();
    }

}
