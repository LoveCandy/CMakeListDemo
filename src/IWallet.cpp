#define WALLET_API_EXPORT
#include "IWallet.h"
#include "wallet_imp.h"

WALLETAPI IWallet* CreateWallet(MessageHandle* message)
{
    Wallet_Imp* pWallet = new Wallet_Imp();
    bool status = pWallet->Register(message);
    if (status == false)
    {
        delete pWallet;
        pWallet = nullptr;
    }
    return pWallet;
}

WALLETAPI bool SetParameter(IWallet * handle, const char * name, const char * value)
{
    return  handle->SetParameter(name, value);
}

WALLETAPI __int64 CreateAccount(IWallet * handle, const char* request_id, CoinType coin_type, const char * username, const char * password)
{
    return handle->CreateAccount(request_id, coin_type, username, password);
}

WALLETAPI __int64 QueryBalance(IWallet* handle, const char* request_id, CoinType coin_type, const char * address)
{
    return handle->QueryBalance(request_id,coin_type, address);
}

WALLETAPI __int64 QueryOrder(IWallet * handle, const char* request_id, CoinType coin_type, const char * address, const Order * p_order)
{
    return handle->QueryOrder(request_id,coin_type, address, p_order);
}

WALLETAPI __int64 QueryOrders(IWallet * handle, const char* request_id, CoinType coin_type, const char * address, const Order * p_order)
{
    return handle->QueryOrders(request_id,coin_type, address, p_order);
}

WALLETAPI __int64 SendCoin(IWallet * handle, const char* request_id, CoinType coin_type, const char * from_address, const char * from_pwd, const char * to_address, const char * description, __int64 amount)
{
    return handle->SendCoin(request_id,coin_type, from_address, from_pwd, to_address, description, amount);
}

WALLETAPI __int64 RecieveCoin(IWallet * handle, const char* request_id, CoinType coin_type, const char* from_address, const char* from_pwd, const char* to_address, const char* description, __int64 amount)
{
    return handle->RecieveCoin(request_id,coin_type, from_address, from_pwd, to_address, description, amount);
}

WALLETAPI void CloseWallet(IWallet * handle)
{
    delete handle;
    handle = nullptr;
}
