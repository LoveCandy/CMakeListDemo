#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <curl/curl.h>
#include <string>
#include "wallet_imp.h"

//
#include <iostream>
//
static size_t write_function(const void *buffer, const size_t size, const size_t nmemb, void *user_p)
{
	char* response_body = (char*)user_p;
	uint32_t response_body_len = strlen(response_body);
	uint32_t len = size * nmemb;
	if (len > RESPONSE_BODY_SIZE - response_body_len - 1) {
		len = RESPONSE_BODY_SIZE - response_body_len - 1;
	}
	memcpy(response_body + response_body_len, buffer, len);

	return size * nmemb;
}

void BeforeGetBalance(const char* MsgID, const char* AccountAddress, std::string &outJson)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	rapidjson::Value str_value(rapidjson::kStringType);
	rapidjson::Value StringArray(rapidjson::kArrayType);

	document.AddMember("jsonrpc", "1.0", allocator);

	str_value.SetString(MsgID, strlen(MsgID));
	document.AddMember("id" , str_value, allocator);

	document.AddMember("method", "getreceivedbyaddress", allocator);

	str_value.SetString(AccountAddress, strlen(AccountAddress));
	StringArray.PushBack(str_value, allocator);
	document.AddMember("params", StringArray, allocator);


	document.Accept(writer);
	outJson =buffer.GetString();
	return;
}
void GenerateResFunc4GetBalance(const char* inputstr, CoinCapital* capital)
{
	rapidjson::Document document;
	const char* strjson = inputstr;
	
	if (document.Parse(strjson).HasParseError())
	{
		capital->amount = -1;//网络错误
		capital->coin_type = CoinType::BTC;
		return;
	}
	rapidjson::Value &jv = document["result"];

	auto &result = document["result"];
	if (result.IsDouble())
	{
		capital->amount = result.GetDouble();
		capital->coin_type = CoinType::BTC;
	}
	else
	{
		capital->amount = 0;//获取不到余额
		capital->coin_type = CoinType::NONE;
	}
	return;
}


void BeforeNewAccount(const char* MsgID, const char* AccountName, std::string &outJson)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();


	document.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value str_MsgID(rapidjson::kStringType);
	str_MsgID.SetString(MsgID, strlen(MsgID));
	document.AddMember("id", str_MsgID, allocator);


	document.AddMember("method", "getnewaddress", allocator);

	rapidjson::Value str_AccountName(rapidjson::kStringType);
	rapidjson::Value StringArray(rapidjson::kArrayType);
	str_AccountName.SetString(AccountName, strlen(AccountName));
	StringArray.PushBack(str_AccountName, allocator);
	document.AddMember("params", StringArray, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	outJson = buffer.GetString();
	return;
}

void GenerateResFunc4NewAccount(const char* inputstr, UserInfo* userinfo, const char * username, const char * password)
{
	rapidjson::Document document;
	//const char* strjson = inputstr;

	if (document.Parse(inputstr).HasParseError())
	{
		strcpy_s(userinfo->username, username);
		strcpy_s(userinfo->password, password);
		strcpy_s(userinfo->address, "0");
		return;
	}
	rapidjson::Value &jv = document["result"];

	auto &result = document["result"];
	if (result.IsString())
	{
		strcpy_s(userinfo->username, username);
		strcpy_s(userinfo->password, password);
		strcpy_s(userinfo->address, result.GetString());
	}
	else
	{
		strcpy_s(userinfo->username, username);
		strcpy_s(userinfo->password, password);
		strcpy_s(userinfo->address, "0");
	}
	return;
}

void BeforeGetOrder(const char* MsgID, const char* TransactionTxid, std::string &outJson)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	document.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value str_MsgID(rapidjson::kStringType);
	str_MsgID.SetString(MsgID, strlen(MsgID));
	document.AddMember("id", str_MsgID, allocator);


	document.AddMember("method", "gettransaction", allocator);

	rapidjson::Value str_AccountName(rapidjson::kStringType);
	rapidjson::Value StringArray(rapidjson::kArrayType);
	str_AccountName.SetString(TransactionTxid, strlen(TransactionTxid));
	StringArray.PushBack(str_AccountName, allocator);
	document.AddMember("params", StringArray, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	outJson =buffer.GetString();
	return;
}

void GenerateResFunc4GetOrder(const char* inputstr,const char* from_address,Order* order)
{
	rapidjson::Document document;
	const char* strjson = inputstr;

	if (document.Parse(strjson).HasParseError())
	{
		strcpy_s(order->description, "NULLRESULT");
		return;
	}


	strcpy_s(order->from_address, "");
	strcpy_s(order->description, "");
	
	auto &result = document["result"];
	if (result.IsObject())
	{

		const auto& confirm_object = result["confirmations"];
		if (confirm_object.IsInt())
		{
			if (confirm_object.GetInt() >= BTC_MINI_CONF)//达到6次确认交易成功
			{
				order->status = OrderStatus::TRADED;
			}
			else
			{
				order->status = OrderStatus::REPORTED;
			}
		}

		const auto& txid_object = result["txid"];
		if (txid_object.IsString())
		{
			strcpy_s(order->order_id, txid_object.GetString());
		}
		const auto& time_object = result["time"];
		if (time_object.IsInt())
		{
			order->order_date = time_object.GetInt();
			order->order_time = time_object.GetInt();
		}



		//order->order_time = result["time"].GetInt();
		const auto& details_Array = result["details"];
		if (details_Array.IsArray())
		{
			const auto& details_object = details_Array[1];

			const auto& amount_object = details_object["amount"];
			if (amount_object.IsDouble())
			{
				order->amount = amount_object.GetDouble() *SatoshiConversionCoefficient; // 需要乘上交易系数
			}
			const auto& address_object = details_object["address"];
			if (address_object.IsString())
			{
				strcpy_s(order->to_address, address_object.GetString());
			}
		}

	}
	else if (result.IsString())
	{
	
	}
	else
	{
		
	}
	return;
}


void BeforeGetAccount(const char* MsgID, const char* from_address,std::string &outJson)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();


	document.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value str_MsgID(rapidjson::kStringType);
	str_MsgID.SetString(MsgID, strlen(MsgID));
	document.AddMember("id", str_MsgID, allocator);


	document.AddMember("method", "getaccount", allocator);

	rapidjson::Value str_AccountName(rapidjson::kStringType);
	rapidjson::Value StringArray(rapidjson::kArrayType);
	str_AccountName.SetString(from_address, strlen(from_address));
	StringArray.PushBack(str_AccountName, allocator);
	document.AddMember("params", StringArray, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	outJson =buffer.GetString();
	return;
}

void GenerateResFunc4GetAccount(const char* inputstr, std::string &from_Account)
{
	rapidjson::Document document;
	//const char* strjson = inputstr;

	if (document.Parse(inputstr).HasParseError())
	{
		//from_Account = "";
		return;
	}
	rapidjson::Value &jv = document["result"];

	auto &result = document["result"];
	if (result.IsString())
	{
		from_Account =result.GetString();
	}
	else
	{
		//from_Account = nullptr;
	}
	return;
}


void BeforeSendfrom(const char* MsgID, const char* from_account, const char* to_address, double amount, std::string &outJson)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();


	document.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value str_MsgID(rapidjson::kStringType);

	str_MsgID.SetString(MsgID, strlen(MsgID));
	document.AddMember("id", str_MsgID, allocator);


	document.AddMember("method", "sendfrom", allocator);

	rapidjson::Value str_AccountName(rapidjson::kStringType);
	rapidjson::Value str_to_address(rapidjson::kStringType);
	rapidjson::Value int_amount(rapidjson::kObjectType);

	rapidjson::Value StringArray(rapidjson::kArrayType);


	str_AccountName.SetString(from_account, strlen(from_account));
	str_to_address.SetString(to_address, strlen(to_address));
	int_amount.SetDouble(amount);
	StringArray.PushBack(str_AccountName, allocator);
	StringArray.PushBack(str_to_address, allocator);
	StringArray.PushBack(int_amount, allocator);
	document.AddMember("params", StringArray, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	outJson = const_cast<char*>(buffer.GetString());
}


void GenerateResFunc4Sendfrom(const char* inputstr, std::string &Trade_Hash)
{
	rapidjson::Document document;
	//const char* strjson = inputstr;

	if (document.Parse(inputstr).HasParseError())
	{
		return;
	}
	

	auto &result = document["result"];
	if (result.IsString())
	{
		Trade_Hash = result.GetString();
	}
	else
	{
	}
	return;
}


void GenerateResFunc4Sendcoin(const char* inputstr, const char* from_address, const char* to_address, Order* order)
{
	rapidjson::Document document;
	const char* strjson = inputstr;

	if (document.Parse(strjson).HasParseError())
	{
		strcpy_s(order->description, "NULL RESULT");
		return;
	}

	strcpy_s(order->from_address, from_address);
	strcpy_s(order->to_address, to_address);
	strcpy_s(order->description, "");

	auto &result = document["result"];
	if (result.IsObject())
	{

		
		const auto& confirm_object = result["confirmations"];
		if (confirm_object.IsInt())
		{
			if (confirm_object.GetInt() >= BTC_MINI_CONF)//达到设定的确认数
			{
				order->status = OrderStatus::TRADED;
			}
			else
			{
				order->status = OrderStatus::REPORTED;
			}
		}

		const auto& txid_object = result["txid"];
		if (txid_object.IsString())
		{
			strcpy_s(order->order_id, txid_object.GetString());
		}
		const auto& time_object = result["time"];
		if (time_object.IsInt())
		{
			order->order_date = time_object.GetInt();
			order->order_time = time_object.GetInt();
		}

		const auto& details_Array = result["details"];
		if (details_Array.IsArray())
		{
			const auto& details_object = details_Array[1];

			const auto& amount_object = details_object["amount"];
			if (amount_object.IsDouble())
			{
				order->amount = amount_object.GetDouble() * SatoshiConversionCoefficient;
			}
		}

	}
	else if (result.IsString())
	{
	}
	else
	{
		
	}
	return;
}


__int64 SendRequestComFunc(const char* BTCServerUrl,const char* ReqBody,std::string &ResponseBody)
{

	const char* url = BTCServerUrl;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: text/plain");
	const char* request_body = ReqBody;
	CURL *curl;
	CURLcode res;
	char response_body[RESPONSE_BODY_SIZE] = { '\0' };
	long response_code = 0;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(request_body));
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_body);
		res = curl_easy_perform(curl);
		if (res == CURLE_OK) {
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		}
		if (res != CURLE_OK) {
			//fprintf(stderr, "request to %s error(%d): %s", url, res, curl_easy_strerror(res));
			return -1;
		}
		curl_easy_cleanup(curl);
	}

	curl_slist_free_all(headers);
	if (response_code == 201) {
		//fprintf(stderr, "request to %s successful: %s\n", url, response_body);
		return -1;
	}

	ResponseBody = response_body;
	return 0;
}



void BeforeGetTranscactionComFunc(const char* MsgID, const char *Trade_Hash, std::string &outputstr)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();


	document.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value str_MsgID(rapidjson::kStringType);
	str_MsgID.SetString(MsgID, strlen(MsgID));
	document.AddMember("id", str_MsgID, allocator);


	document.AddMember("method", "gettransaction", allocator);

	rapidjson::Value str_AccountName(rapidjson::kStringType);
	rapidjson::Value StringArray(rapidjson::kArrayType);
	str_AccountName.SetString(Trade_Hash, strlen(Trade_Hash));
	StringArray.PushBack(str_AccountName, allocator);
	document.AddMember("params", StringArray, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	outputstr = buffer.GetString();
	return;
}