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
    //���سɹ����
    virtual bool SetParameter(const char* name, const char* value) { return false; };
    //����״̬id
    virtual __int64 CreateAccount(const char* request_id, CoinType coin_type, const char* username, const char* password) { return 0; };
    //����״̬id
    virtual __int64 QueryBalance(const char* request_id, CoinType coin_type, const char* address) { return 0; };
    //����״̬id
    virtual __int64 QueryOrder(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) { return 0; };
    //����״̬id
    virtual __int64 QueryOrders(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) { return 0; };
    //����״̬id
    virtual __int64 SendCoin(const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount) { return 0; };
    //����request_id
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

/*******************************************************************  �ص����� *********************************************************************/
//�ʽ��ѯ�ص�����
typedef void(*Wallet_CreateAccountHandler)(IWallet* handle, const char* request_id, const UserInfo* account, int errorCode, const char* error);
//�ʽ��ѯ�ص�����
typedef void(*Wallet_BalanceHandler)(IWallet* handle, const char* request_id, const CoinCapital* capital, int errorCode, const char* error);
//�����ص�
typedef void(*Wallet_OrderHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//�����б�ص�
typedef void(*Wallet_OrdersHandler)(IWallet* handle, const char* request_id, const Order* orders, size_t length, int errorCode, const char* error);
//�ɽ��ص�
typedef void(*Wallet_TradedHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//ȡ�������ص�
typedef void(*Wallet_CanceledHandler)(IWallet* handle, const char* request_id, const Order* order, int errorCode, const char* error);
//�ܵ��ص�
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

    //��þ��
    WALLETAPI IWallet* CreateWallet(MessageHandle* message);
    //���ò���
    WALLETAPI bool SetParameter(IWallet* handle, const char* name, const char* value);
    //��þ��
    WALLETAPI __int64 CreateAccount(IWallet* handle, const char* request_id, CoinType coin_type, const char* username, const char* password);
    //��ѯ�ʽ�
    WALLETAPI __int64 QueryBalance(IWallet* handle, const char* request_id, CoinType coin_type, const char* address);
    //��ѯ����
    WALLETAPI __int64 QueryOrder(IWallet* handle, const char* request_id, CoinType coin_type, const char* address, const Order* p_order);
    //��ѯ�����б�
    WALLETAPI __int64 QueryOrders(IWallet* handle, const char* request_id, CoinType coin_type, const char* address, const Order* p_order);
    //���ͻ���
    WALLETAPI __int64 SendCoin(IWallet* handle, const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount);
    //���ջ���
    WALLETAPI __int64 RecieveCoin(IWallet* handle, const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount);
    //�ͷ�Ǯ�����
    WALLETAPI void CloseWallet(IWallet* handle);

#ifdef __cplusplus
}
#endif

#endif // !I_WALLET_H