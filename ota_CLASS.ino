int OTAUpdater::checkUrlExists(const String &firmwareURL)
{
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println("OTA: La red no está conectada");
		return false;
	}

	HTTPClient http;
	http.begin(firmwareURL);
	int httpCode = http.sendRequest("HEAD");

	Serial.println("OTA: Comprobando existencia de '" + firmwareURL + "'");
	if (httpCode == HTTP_CODE_OK)
	{
		Serial.println("OTA: '" + firmwareURL + "' OK");
		http.end();
		return 200;
	}
	else
	{
		Serial.println("OTA: '" + firmwareURL  + "' http-code: " + String(httpCode));
		http.end();
		return httpCode;
	}
}


void OTAUpdater::_update(const String &firmwareURL)
{
	bool ledBlink = false;
	String msg;

	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println("OTA: La red no está conectada");
		return;
	}

	Serial.println("OTA: Iniciando descarga de '" + firmwareURL + "'");

	HTTPClient http;
	http.begin(firmwareURL);

	int httpCode = http.GET();
	if (httpCode == HTTP_CODE_OK)
	{
		size_t contentLength = http.getSize();
		Serial.println("OTA: Tamaño del Firmware: " + String(contentLength));

		// Verificación de tamaño de contenido válido
		if (contentLength <= 0)
		{
			Serial.println("OTA: Tamaño de firmware no válido");
			http.end();
			return;
		}

		// Verificación de espacio disponible
		if (contentLength > ESP.getFreeSketchSpace())
		{
			Serial.println("OTA: No hay suficiente espacio en flash para la actualización");
			http.end();
			return;
		}

		if (Update.begin(contentLength))
		{
			Serial.println("OTA: Descargando");
			// Callback para mostrar el progreso en el puerto serial
			Update.onProgress([&ledBlink](size_t progress, size_t total)
			{
				ledBlink = !ledBlink;
				led_wifi(ledBlink);
				led_loop(ledBlink);
				led_error(ledBlink);
				int progressPercentage = (progress * 100) / total;
				Serial.println("Descargando " + String(progressPercentage));
			});

			// Escribir el contenido descargado en el firmware
			WiFiClient *stream = http.getStreamPtr();
			if (Update.writeStream(*stream))
			{
				// Finalizar la actualización y verificar la integridad
				if (Update.end(true))
				{
					Serial.println("OTA: Actualización completada con éxito...");
					reboot();
				}
				else
				{
					Serial.println("OTA: Se ha finalizado la descarga, pero no se aplica por error en la verificación de la integridad");
				}
			}
			else
			{
				Serial.println("OTA: Error subiendo el firmware");
			}
		}
		else
		{
			Serial.println("OTA: No se pudo comenzar la actualización");
		}
	}
	else
	{
		Serial.println("OTA: http-code " + String(httpCode) + ". No se pudo descargar '" + firmwareURL + "'");
	}
	http.end(); // Cierra la conexión HTTP
}

void OTAUpdater::updateUrl(String &firmwareURL)
{
	_update(firmwareURL);
}
