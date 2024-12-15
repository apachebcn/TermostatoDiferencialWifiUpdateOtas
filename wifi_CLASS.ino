void myWifi::wifi_disconnect()
{
	WiFi.disconnect();
	wifi_led();
}

void myWifi::wifi_connect(byte reintentos, bool fromSetup)
{
	// No impedir el WiFi.begin bajo ningún concepto
	// De lo contrario el esp32 petará en cualquier accion de red
	// Por ejemplo, en el setup el "server.begin()" petará si no se a ejectuado WiFi.begin

	String ssid = configVar.getStringByName("CONFIG_NETWORK_WIFI_SSID");
	String pass = configVar.getStringByName("CONFIG_NETWORK_WIFI_PASSWORD");
	String hostname = configVar.getStringByName("CONFIG_NETWORK_HOSTNAME");
	String msg;

	// Seteo el hostname
	if (hostname)
	{
		WiFi.setHostname(hostname.c_str());
		if (MDNS.begin(hostname))
		{
			//Serial.println("MDNS responder started");
			MDNS.addService("http", "tcp", 80);
		}
	}
	WiFi.mode(WIFI_STA);
	if (fromSetup) 
	{
		Serial.println("WIFI-CONNECT MAC de este dispositivo: " + String(WiFi.macAddress()));
	}
	WiFi.begin(ssid, pass);  // Aunque ssid y pass no tengan valor, hago igualmente el WiFi.begin porque si no la capa tcp/ip peta
	setup_dinamic_ip(fromSetup);

	if (!ssid.length())
	{
		if (fromSetup)
			Serial.println(F("WIFI-CONNECT ERROR: config wifi ssid null"));
		else
			Serial.println(F("WIFI-CONNECT: config wifi ssid null"));
		wifi_disconnect();
		wifi_led();
		return;
	}

	if (!pass.length())
	{
		if (fromSetup)
			Serial.println(F("WIFI-CONNECT ERROR: config wifi password null"));
		else
			Serial.println(F("WIFI-CONNECT: config wifi password null"));
		wifi_disconnect();
		wifi_led();
		return;
	}

	if (reintentos)
	{
		byte times = 1;
		bool led_wifi_value = false;
		led_wifi(0);
		while (WiFi.status() != WL_CONNECTED && times < reintentos)
		{
			times++;
			if (WiFi.status() != WL_CONNECTED && times < reintentos)
			{
				msg = "Conectando a red wifi '" + ssid + "' (" + String(times) + "/" + String(reintentos) + " segundos)";
				if (fromSetup)
					Serial.println("WIFI-CONNECT ERROR:" + msg);
				else
					Serial.println("WIFI-CONNECT:" + msg);
			}
			led_wifi_value = !led_wifi_value;
			led_wifi(led_wifi_value);
			delay(1000);
		}
		led_wifi(0);
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		msg = wifi_info();
		if (fromSetup)
			Serial.println("WIFI-CONNECT: " + String(msg));
		else
			Serial.println("WIFI-CONNECT: " + String(msg));
	}
	else
	{
		msg = "No connected to " + ssid;
		if (fromSetup)
			Serial.println("WIFI-CONNECT ERROR: " + String(msg));
		else
			Serial.println("WIFI-CONNECT: " + String(msg));
	}
	wifi_led();
}

void myWifi::setup_dinamic_ip(bool fromSetup)
{
	if (configVar.getBooleanByName("CONFIG_NETWORK_DINAMIC_ENABLE"))
	{
		String configDinamicIp = configVar.getStringByName("CONFIG_NETWORK_DINAMIC_IP");
		String configDinamicMask = configVar.getStringByName("CONFIG_NETWORK_DINAMIC_MASK");
		String configGateway = configVar.getStringByName("CONFIG_NETWORK_DINAMIC_GATEWAY");
		String configDinamicDns = configVar.getStringByName("CONFIG_NETWORK_DINAMIC_DNS");
		if (!configDinamicIp.length())
		{
			if (fromSetup)
				Serial.println(F("WIFI-CONNECT ERROR: null CONFIG_NETWORK_DINAMIC_IP"));
			else
				Serial.println(F("WIFI-CONNECT: null CONFIG_NETWORK_DINAMIC_IP"));
			return;
		}
		if (!configDinamicMask.length())
		{
			if (fromSetup)
				Serial.println(F("WIFI-CONNECT ERROR: null CONFIG_NETWORK_DINAMIC_MASK"));
			else
				Serial.println(F("WIFI-CONNECT: null CONFIG_NETWORK_DINAMIC_MASK"));
			return;
		}
		if (!configGateway.length())
		{
			if (fromSetup)
				Serial.println(F("WIFI-CONNECT ERROR: null CONFIG_NETWORK_DINAMIC_GATEWAY"));
			else
				Serial.println("WIFI-CONNECT: null CONFIG_NETWORK_DINAMIC_GATEWAY");
			return;
		}

		IPAddress local_IP, gateway, subnet, dns;
		dns.fromString(configDinamicDns);
		local_IP.fromString(configDinamicIp);
		subnet.fromString(configDinamicMask);
		gateway.fromString(configGateway);
		if (!WiFi.config(local_IP, gateway, subnet, dns))
		{
			if (fromSetup)
				Serial.println("WIFI-CONNECT ERROR: Setup with dinamic IP '" + String(configDinamicIp) + "'");
			else
				Serial.println("WIFI-CONNECT: Setup with dinamic IP '" + String(configDinamicIp) + "'");
		}
	}
}

String myWifi::wifi_info()
{
	String info = "";
	IPAddress localIP, gatewayIP, subnet, dns;
	localIP = WiFi.localIP();
	gatewayIP = WiFi.gatewayIP();
	subnet = WiFi.subnetMask();
	dns = WiFi.dnsIP();
	String IPString = localIP.toString();
	String subnetString = subnet.toString();
	String gatewayIPString = gatewayIP.toString();
	String dnsString = dns.toString();
	info += "Wifi con Mac(" + String(WiFi.macAddress()) + ") conectado a:\n";
	info += " SSID: " + String(WiFi.SSID()) + "\n";
	info += " Canal: " + String(WiFi.channel()) + "\n";
	info += " Intensidad: " + String(WiFi.RSSI()) + "\n";
	info += " Ip: " + String(IPString) + "\n";
	info += " Subnet: " + String(subnetString) + "\n";
	info += " Gateway: " + String(gatewayIPString) + "\n";
	info += " Dns: " + String(dnsString);
	return info;
}

bool myWifi::isValidIP(const String &str)
{
	// Verificar si hay exactamente tres puntos en la dirección IP
	int dotCount = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str.charAt(i) == '.')
		{
			dotCount++;
		}
	}
	if (dotCount != 3)
	{
		return false;
	}

	// Verificar cada octeto
	int lastDotPos = -1;
	for (int i = 0; i < 4; i++)
	{
		int dotPos = str.indexOf('.', lastDotPos + 1);
		if (dotPos == -1)
		{
			dotPos = str.length(); // Último octeto
		}

		String octet = str.substring(lastDotPos + 1, dotPos);
		if (octet.length() == 0 || octet.length() > 3)
		{
			return false; // Cada octeto debe tener de 1 a 3 dígitos
		}

		// Verificar si todos los caracteres del octeto son dígitos
		for (int j = 0; j < octet.length(); j++)
		{
			if (!isDigit(octet.charAt(j)))
			{
				return false;
			}
		}

		// Convertir el octeto a un número y verificar el rango
		int octetValue = octet.toInt();
		if (octetValue < 0 || octetValue > 255)
		{
			return false;
		}

		lastDotPos = dotPos;
	}

	return true;
}


void myWifi::wifi_led()
{
	if (WiFi.status() == WL_CONNECTED)
		led_wifi(1);
	else
		led_wifi(0);
}


bool myWifi::sendUrl(char* url, String &payload, const __FlashStringHelper* logName)
{
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println("Network disconnected");
		return false;
	}

	int httpCode = 0;
	HTTPClient http;

	http.setConnectTimeout(5000);
	http.setTimeout(5000);
	http.addHeader("Content-Type", "application/x-www-form-urlencoded");
	// http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); // Configurar para seguir redirecciones
	http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

	try
	{
		unsigned long time1 = millis();
		http.begin(url);
		httpCode = http.GET();

		payload = http.getString();
		http.end();

		unsigned long time2 = millis();
		unsigned long timeProcesed;
		if (time2 < time1)
		{
			timeProcesed = (ULONG_MAX - time1) + time2 + 1;
		}
		else
		{
			timeProcesed = time2 - time1;
		}		

		Serial.println(String(logName) +  " HTTP-" + String(httpCode) + " en " + String(timeProcesed) + "ms. Url: '" + String(url) + "'");
		if (httpCode > 199 && httpCode < 300)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(const std::exception& e)
	{
		Serial.println("Excepción: " + String(e.what()) + ". Url: '" + String(url));
		return false;
	}
}
