//#include "stdafx.h"
#include "wallet_imp.h"
#include <string>

//
#include <iostream>
//
//1
void BeforeGetBalance(const char* MsgID, const char* AccountAddress, std::string &outJson);
void GenerateResFunc4GetBalance(const char* inputstr, CoinCapital* capital);
//2
void BeforeNewAccount(const char* MsgID, const char* AccountAddress, std::string & outJson);
void GenerateResFunc4NewAccount(const char* inputstr, UserInfo* userinfo,const char * username,const char * password);
//3
void BeforeGetAccount(const char* MsgID, const char* from_address, std::string &outJson);
void GenerateResFunc4GetAccount(const char* inputstr, std::string &from_Account);

void BeforeSendfrom(const char* MsgID, const char* from_account, const char* to_address, double amount, std::string &outJson);
void GenerateResFunc4Sendfrom(const char* inputstr, std::string &Trade_Hash);
void GenerateResFunc4Sendcoin(const char* inputstr, const char* from_address, const char* to_address, Order* order);

//4
void BeforeGetOrder(const char* MsgID, const char* TransactionTxid, std::string &outJson);
void GenerateResFunc4GetOrder(const char* inputstr, const char* from_address, Order* order);


__int64 GetTranscactionComFunc(const char* request_id, const char *Trade_Hash, std::string &outputstr);
void BeforeGetTranscactionComFunc(const char* MsgID, const char *Trade_Hash, std::string &outputstr);
__int64 SendRequestComFunc(const char* BTCServerUrl,const char* ReqBody,std::string &ResponseBody);

std::string gServer;

Wallet_Imp::Wallet_Imp()
{
}


Wallet_Imp::~Wallet_Imp()
{
}

bool Wallet_Imp::Register(MessageHandle * handle)
{
	if (handle_ == nullptr)
		return false;
	handle_ = handle;
	return true;
}

bool Wallet_Imp::SetParameter(const char * name, const char * value)
{
	if (NULL == value)
	{
		return false;
	}
	gServer = value;
    return true;
}

__int64 Wallet_Imp::CreateAccount(const char* request_id, CoinType coin_type, const char * username, const char * password)
{
	std::string result;
	std::string ResponseBody;
	UserInfo userinfo;
	ResponseCode rescode;
	__int64 res = 0;
	if (NULL == request_id || NULL == username)
	{
		handle_->on_create_account_handle(this, request_id, &userinfo,rescode.REQ_CODE_INVAIDARG, rescode.REQ_MSG_INVAIDARG);
		return -1;
	}
	BeforeNewAccount(request_id, username, result);
	if (0 != SendRequestComFunc(gServer.c_str(), result.c_str(), ResponseBody))
	{
		handle_->on_create_account_handle(this, request_id, &userinfo, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}

	GenerateResFunc4NewAccount(ResponseBody.c_str(), &userinfo, username, password);

	handle_->on_create_account_handle(this, request_id, &userinfo, rescode.REQ_CODE_SUCCESS, rescode.REQ_MSG_SUCCESS);


    return 0;
}

__int64 Wallet_Imp::QueryBalance(const char* request_id, CoinType coin_type, const char * address)
{
	std::string result;
	std::string ResponseBody;
	CoinCapital capital;
	ResponseCode rescode;
	__int64 res = 0;
	if (NULL == request_id || NULL == address)
	{
		handle_->on_balance_handle(this, request_id, &capital, rescode.REQ_CODE_INVAIDARG,rescode.REQ_MSG_INVAIDARG);
		return -1;
	}
	BeforeGetBalance(request_id, address, result);
	if (0 != SendRequestComFunc(gServer.c_str(), result.c_str(), ResponseBody))
	{
		handle_->on_balance_handle(this, request_id, &capital, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}

	GenerateResFunc4GetBalance(ResponseBody.c_str(),&capital);

	handle_->on_balance_handle(this, request_id, &capital, rescode.REQ_CODE_SUCCESS, rescode.REQ_MSG_SUCCESS);


	return 0;
}

__int64 Wallet_Imp::QueryOrder(const char* request_id, CoinType coin_type, const char * address, const Order * p_order)
{
	std::string result;
	std::string ResponseBody;
	Order order;
	ResponseCode rescode;
	__int64 res = 0;
	if (NULL == request_id || NULL == p_order)
	{
		handle_->on_order_handle(this, request_id, &order, rescode.REQ_CODE_INVAIDARG, rescode.REQ_MSG_INVAIDARG);
		return -1;
	}
	BeforeGetTranscactionComFunc(request_id, address, result);
	if (0 != SendRequestComFunc(gServer.c_str(), result.c_str(), ResponseBody))
	{
		handle_->on_order_handle(this, request_id, &order, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}
	
	GenerateResFunc4GetOrder(ResponseBody.c_str(), address,&order);

	handle_->on_order_handle(this, request_id, &order, rescode.REQ_CODE_SUCCESS, rescode.REQ_MSG_SUCCESS);


	
	return 0;
}

__int64 Wallet_Imp::QueryOrders(const char* request_id, CoinType coin_type, const char * address, const Order * p_order)
{
	return 0;
}

__int64 Wallet_Imp::SendCoin(const char* request_id, CoinType coin_type, const char * from_address, const char * from_pwd, const char * to_address, const char * description, __int64 amount)
{
	std::string outJsontemp;
	std::string ResponseBody;
	std::string ResponseBody2;
	std::string from_acount;
	std::string Trade_Hash;
	std::string ResponseBody3;
	Order order;//交易
	ResponseCode rescode;
	__int64 res = 0;
	double famount = (double)amount / SatoshiConversionCoefficient;

	if (NULL == request_id || NULL == from_address || NULL == to_address || MIN_VALUE >= famount)
	{
		handle_->on_traded_handle(this, request_id, &order, rescode.REQ_CODE_INVAIDARG, rescode.REQ_MSG_INVAIDARG);
		return -1;
	}
	//step1:通过源地址获取账户名
	BeforeGetAccount(request_id, from_address, outJsontemp);
	if (0 != SendRequestComFunc(gServer.c_str(), outJsontemp.c_str(), ResponseBody))
	{
		handle_->on_traded_handle(this, request_id, &order, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}

	GenerateResFunc4GetAccount(ResponseBody.c_str(), from_acount);

	
	 //step2 发送交易
	outJsontemp.clear();
	BeforeSendfrom(request_id, from_acount.c_str(),to_address, famount, outJsontemp);
	if (0 != SendRequestComFunc(gServer.c_str(), outJsontemp.c_str(), ResponseBody2))
	{
		handle_->on_traded_handle(this, request_id, &order, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}
	GenerateResFunc4Sendfrom(ResponseBody2.c_str(), Trade_Hash);

	//step3 获取交易具体信息
	if (0 != GetTranscactionComFunc(request_id, Trade_Hash.c_str(), ResponseBody3))
	{
		handle_->on_traded_handle(this, request_id, &order, rescode.REQ_CODE_NET_UNREACHABLE, rescode.REQ_MSG_NET_UNREACHABLE);
		return -1;
	}

	GenerateResFunc4Sendcoin(ResponseBody3.c_str(),from_address,to_address, &order);

	handle_->on_traded_handle(this, request_id, &order, rescode.REQ_CODE_SUCCESS, rescode.REQ_MSG_SUCCESS);
	return 0;
}

__int64 Wallet_Imp::RecieveCoin(const char* request_id, CoinType coin_type, const char * from_address, const char * from_pwd, const char * to_address, const char * description, __int64 amount)
{
	return 0;
}




__int64 GetTranscactionComFunc(const char* request_id, const char *Trade_Hash, std::string &outputstr)
{
	std::string result;
	std::string ResponseBody;
	int res = 0;
	if (nullptr == Trade_Hash)
	{
		return -1;
	}
	BeforeGetTranscactionComFunc(request_id, Trade_Hash,result);
	if (0 != SendRequestComFunc(gServer.c_str(), result.c_str(), ResponseBody))
	{
		return -1;
	}
	outputstr = ResponseBody;
	return 0;

}