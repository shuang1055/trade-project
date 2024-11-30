// trade-core-console.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"

// thread 1: UI thread.
// thread 2: logic thread.
// thread 3: CTP thread.


char g_mdFront[] = "tcp://121.37.80.177:20004";


class CSimpleHandler : public CThostFtdcMdSpi
{
private:
    CThostFtdcMdApi* m_pUserApi;

public:
    //CSimpleHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi){}
    CSimpleHandler(CThostFtdcMdApi* pUserApi)
    {
        this->m_pUserApi = pUserApi;
    };

    ~CSimpleHandler() {}

    void onFrontConnected(char* BrokerID, char* UserID, char* Password)
    {
        // this function is called in thread 3.
        CThostFtdcReqUserLoginField reqUserLogin;
        strcpy(reqUserLogin.BrokerID, BrokerID);
        strcpy(reqUserLogin.UserID, UserID);
        strcpy(reqUserLogin.Password, Password);
        this->m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
        
    }

    void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
        CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
    {
        if (pRspInfo->ErrorID == 0) {
            printf("Login successful!\n");
        }
        else {
            printf("Login failed: %s\n", pRspInfo->ErrorMsg);
        }
    }
    
};


int main()
{
    CThostFtdcMdApi *api = CThostFtdcMdApi::CreateFtdcMdApi();
    CSimpleHandler handler(api);
    api->RegisterSpi(&handler);
    api->RegisterFront(g_mdFront);
    api->Init();
    api->Join();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
