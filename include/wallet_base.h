/**
 * 作者：邱敏
 *
 * 涉及资金的变量均按最小单位取整；取消浮点数干扰
 *
 */
#ifndef WALLET_BASE_H
#define WALLET_BASE_H

#pragma pack(push)
#pragma pack(1)

const int NAME_LENGTH = 32;
const int PASSWORD_LENGTH = 128;
const int ADDRESS_LENGTH = 128;
const int DESCRIPTION_LENGTH = 128;
const int ORDER_ID_LENGTH = 128;//64bit内存不足
const int RESPONSE_BODY_SIZE = 4096;
const int SatoshiConversionCoefficient = 1000000000;
const int BTC_MINI_CONF = 6;
const double MIN_VALUE = 1e-8;
enum class CoinType
{
    NONE = 0,
    BTC = 1,
    ETH = 2,
    BUSD = 3,

};

enum class OrderStatus
{
    NONE = 0,
    TO_BE_REPORTED = 1,
    REPORTED = 2,
    PARTIAL_TRATED = 3,
    PARTIAL_CANCELED = 4,
    TRADED = 5,
    REJECTED = 6,

};

struct Order
{
    char order_id[ORDER_ID_LENGTH];
    char from_address[ADDRESS_LENGTH];
    char to_address[ADDRESS_LENGTH];
    char description[DESCRIPTION_LENGTH];
    __int64 amount;
    int order_date;
    int order_time;
    OrderStatus status;

};

struct UserInfo
{
    char username[NAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char address[ADDRESS_LENGTH];
};

struct CoinInfo
{
    CoinType type;
    char ch_name[NAME_LENGTH];
    char en_name[NAME_LENGTH];
    double base_fee;

};

struct CoinCapital
{
    CoinType coin_type;
    __int64 amount;
    __int64 frozen_amount;

};

struct ResponseCode
{
public:

	const char *REQ_MSG_SUCCESS;
	const char *REQ_MSG_NET_UNREACHABLE;
	const char *REQ_MSG_INVAIDARG;
	int REQ_CODE_SUCCESS = 2000;
	int REQ_CODE_NET_UNREACHABLE = 2001;
	int REQ_CODE_INVAIDARG = 2002;

	ResponseCode()
	{
		REQ_MSG_SUCCESS = "OK";
		REQ_MSG_NET_UNREACHABLE = "NET_UNREACHABLE";
		REQ_MSG_INVAIDARG = "INVALD_ARG";
	}
};


#pragma pack(pop)

#endif // !WALLET_BASE_H
