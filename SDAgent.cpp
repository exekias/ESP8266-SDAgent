#include "SDAgent.h"

// api.serverdensity.io SSL Fingerprint
#define SSL_FINGERPRINT "64 1A AF 87 8E BE 1D E8 92 B8 5B F2 DF A2 47 70 B5 2C EA D2"


String URLEncode(String data);


SDAgent::SDAgent(String token, String account, String agent_key)
{
	sd_token = token;
	sd_agent_key = agent_key;

	// Account host
	sd_account_host = account + ".serverdensity.io";
}


int SDAgent::send(String payload)
{

	// URL
	String URL = "https://api.serverdensity.io/alerts/postbacks?token=" + String(sd_token);

	// Compute payload (repalce first `{` with agentKey info
	String fullPayload = "{\"agentKey\":\"" + sd_agent_key + "\"," + payload.substring(1);

	// Hash payload
	unsigned char* hash=MD5::make_hash((char *)fullPayload.c_str());
	char *md5 = MD5::make_digest(hash, 16);

	// Body form
	String body = "hash=" + String(md5) + "&payload=" + URLEncode(fullPayload);
	Serial.println(fullPayload);
	Serial.println("Encoded: " + body);

	// Do the request:
	HTTPClient client;
	Serial.println("X-Forwarded-Host: " + sd_account_host);
	Serial.print("Sending to " + URL + "...");
	client.begin(URL, SSL_FINGERPRINT);
	client.addHeader(F("X-Forwarded-Host"), sd_account_host, true);
	int res = client.POST(body);

	// Handle result:
	if (res == HTTP_CODE_OK)
	{
		Serial.println("OK");
	}
	else
	{
		Serial.println("ERROR");
		Serial.println(client.errorToString(res));
	}
	return res;
}


String URLEncode(String data)
{
	const char *hex = "0123456789abcdef";
	char *msg = (char *)data.c_str();
	String encodedMsg = "";

	while (*msg!='\0')
	{
		if( ('a' <= *msg && *msg <= 'z')
		     || ('A' <= *msg && *msg <= 'Z')
		     || ('0' <= *msg && *msg <= '9') )
		{
			 encodedMsg += *msg;
		}
		else
		{
			encodedMsg += '%';
			encodedMsg += hex[*msg >> 4];
			encodedMsg += hex[*msg & 15];
		}
		msg++;
	}

	return encodedMsg;
}
