#if !defined(WIFI_CLASS_H)
#define WIFI_CLASS_H

	#define TIME_WIFI_ALIFE_SECONDS 10
	#define WIFI_RECONNECT_TIMES 10

	extern unsigned long wifiAliveTestNextTime = 0;
	
	class myWifi
	{

		public:

			static void wifi_disconnect();
			static void wifi_connect(byte reintentos, bool fromSetup);
			static void setup_dinamic_ip(bool printInSerial);
			static String wifi_info();
			static bool isValidIP(const String &str);
			static void wifi_led();
			static bool sendUrl(char* url, String &payload, const __FlashStringHelper* logName);
	};
#endif