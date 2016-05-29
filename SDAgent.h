#ifndef SDAgent_h
#define SDAgent_h

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <MD5.h>


class SDAgent
{
public:
	SDAgent(String token, String account, String agent_key);

	// Send given JSON dictionary as payload
	int send(String payload);

private:
	String sd_token, sd_agent_key, sd_account_host;
};

#endif
