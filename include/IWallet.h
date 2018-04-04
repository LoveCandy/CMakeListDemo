#ifndef I_WALLET_H
#define I_WALLET_H

#include "wallet_base.h"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#define __PLATFORM_WINDOWS__
#else
#define __PLATFORM_LINUX__
#endif

#ifdef __PLATFORM_WINDOWS__
#ifdef WALLET_API_EXPORT
#define WALLETAPI __declspec(dllexport)
#else    
#define WALLETAPI __declspec(dllimport)
#endif
#else
#define WALLETAPI
#endif

class Wallet_Imp;

class IWallet
{
public:
    IWallet() {};
    virtual ~IWallet() {};
    //返回成功与否
    virtual bool SetParameter(const char* name, const char* value) { return false; };
    //返回状态id
    virtual __int64 CreateAccount(const char* request_id, CoinType coin_type, const char* username, const char* password) { return 0; };
    //返回状态id
    virtual __int64 QueryBalance(const char* request_id, CoinType coin_type, const char* address) { return 0; };
    //返回状态id
    virtual __int64 QueryOrder(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) { return 0; };
    //返回状态id
    virtual __int64 QueryOrders(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) { return 0; };
    //返回状态id
    virtual __int64 SendCoin(const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount) { return 0; };
    //返回request_id
    virtual __int64 RecieveCoin(const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount) { return 0; };

    virtual void OnCreateAccount(const char* request_id, UserInfo* user, int errorCode, const char* error) {};
	virtual void OnQueryBalance(const char* request_id, const CoinCapital* capital, int errorCode, const char* error) {};
    virtual void OnQueryOrder(const char* request_id, const Order* order, int errorCode, const char* error) {};
    virtual void OnQueryOrders(const char* request_id, const Order* orders, size_t length, int errorCode, const char* error) {};
    virtual void OnTraded(const char* request_id, const Order* order, int errorCode, const char* error) {};
    virtual void OnCanceled(const char* request_id, const Order* order, int errorCode, const char* error) {};
    virtual void OnRejected(const char* request_id, const Order* order, int errorCode, const char* error) {};

private:
    Wallet_Imp * imp_;
};

/*******************************************************************  回调函数 *********************************************************************/
//资金查询回调函数
typedef void(*Wallet_CreateAccountHandler)(IWallet* handle, const char* request_id, const UserInfo* account, int errorCode, const char* error);
//资金查询回调函数
typedef void(*Wallet_BalanceHandler)(IWallet* handle, const char* request_id, const CoinCapital* capital, int errorCode, const char* error);
//订单回调
typedef void(*Wallet_OrderHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//订单列表回调
typedef void(*Wallet_OrdersHandler)(IWallet* handle, const char* request_id, const Order* orders, size_t length, int errorCode, const char* error);
//成交回调
typedef void(*Wallet_TradedHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//取消订单回调
typedef void(*Wallet_CanceledHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//拒单回调
typedef void(*Wallet_RejectedHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);

struct MessageHandle
{
    Wallet_CreateAccountHandler on_create_account_handle;
    Wallet_BalanceHandler on_balance_handle;
    Wallet_OrderHandler on_order_handle;
    Wallet_OrdersHandler on_orders_handle;
    Wallet_TradedHandler on_traded_handle;
    Wallet_CanceledHandler on_canceled_handle;
    Wallet_RejectedHandler on_rejected_handle;
};


#ifdef __cplusplus
extern "C"
{
#endif

    //获得句柄
    WALLETAPI IWallet* CreateWallet(MessageHandle* message);
    //设置参数
    WALLETAPI bool SetParameter(IWallet* handle, const char* name, const char* value);
    //获得句柄
    WALLETAPI __int64 CreateAccount(IWallet* handle, const char* request_id, CoinType coin_type, const char* username, const char* password);
    //查询资金
    WALLETAPI __int64 QueryBalance(IWallet* handle, const char* request_id, CoinType coin_type, const char* address);
    //查询订单
    WALLETAPI __int64 QueryOrder(IWallet* handle, const char* request_id, CoinType coin_type, const char* address, const Order* p_order);
    //查询订单列表
    WALLETAPI __int64 QueryOrders(IWallet* handle, const char* request_id, CoinType coin_type, const char* address, const Order* p_order);
    //发送货币
    WALLETAPI __int64 SendCoin(IWallet* handle, const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount);
    //接收货币
    WALLETAPI __int64 RecieveCoin(IWallet* handle, const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount);
    //释放钱包句柄
    WALLETAPI void CloseWallet(IWallet* handle);

#ifdef __cplusplus
}
#endif

#endif // !I_WALLET_H