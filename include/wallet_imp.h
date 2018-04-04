#ifndef WALLET_IMP_H
#define WALLET_IMP_H

#include "wallet_base.h"
#include "IWallet.h"

class Wallet_Imp
    :public IWallet
{
public:
    Wallet_Imp();
    ~Wallet_Imp();

    bool Register(MessageHandle* handle);
    //���سɹ����
    bool SetParameter(const char* name, const char* value) override;
    //����request_id
    __int64 CreateAccount(const char* request_id, CoinType coin_type, const char* username, const char* password) override;
    //����request_id
    __int64 QueryBalance(const char* request_id, CoinType coin_type, const char* address) override;
    //����request_id
    __int64 QueryOrder(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) override;
    //����request_id
    __int64 QueryOrders(const char* request_id, CoinType coin_type, const char* address, const Order* p_order) override;
    //����request_id
    __int64 SendCoin(const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount) override;
    //����request_id
    __int64 RecieveCoin(const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount) override;
private:
    MessageHandle * handle_;
};

#endif // !WALLET_IMP_H
