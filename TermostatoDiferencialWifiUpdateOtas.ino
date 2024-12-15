/*
	SOFTWARE PARA ESP32
	PARA SUBIR DE FORMA INTERACTIVA CUALQUIER FIRMWARE
	ESTÁ PENSADO PARA LOS TERMOSTATOS DIFERENCIALES DE MISOLARCASERO.COM

	Url actual para termostato server: https://ota.termostatodiferencial.com/Server/Firmware.bin
	Url actual para termostato visor: https://ota.termostatodiferencial.com/Visor/Firmware.bin

	Cuando se introduce este firmware, el termostato tiene que mostrar el led Loop de forma intermitente por cada 1000ms
	Se configura los parametros desde la consola de arduino.
	"help" muestra la lista de comandos disponibles.
	"config list" muestra la lista de variables configurables.

	Generalmente se consigue el update con los comandos que listo abajo.
	Como idea y consegjo, puedes escribirlo todo en un editor, e insertarlo en la consola haciendo copy y paste.
	Si haces esto que te aconsejo, será fácil y si necesitas repetirlo no tendrás que reescribir.

	Comandos para hacer el update:
		CONFIG SET CONFIG_OTA_URL=url del firmware
		CONFIG CONFIG_NETWORK_WIFI_SSID=tu nombre de wifi
		CONFIG CONFIG_NETWORK_WIFI_PASSWORD=tu password de wifi
		WIFI CONNECT
		OTA UPDATE

	Si necesitas ip dinámica también se puede ajustando la configuración con los comandos.
*/

#include <Arduino.h>
#include <Update.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>

#include "wifi_CLASS.h"
#include "ota_CLASS.h"
#include "serial_input_command_CLASS.h"
#include "config_var_CLASS.h"

#define PIN_LED_WIFI 2
#define PIN_LED_LOOP 4
#define PIN_LED_ERROR 15

bool led_loop_blink_state = false;
unsigned long lastLedToggleTime = 0;
bool updateOta = false;

void setupSerialSaludo()
{
	Serial.println();
	Serial.println(F("ACTUALIZADOR DE SOFTWARE PARA TERMOSTATO DIFERENCIAL Y DISPOSITIVOS"));
	Serial.println();
	Serial.println(F("apachebcn@gmail.com"));
	Serial.println(F("-misolarcasero.com-"));
	Serial.println();
}

void setup()
{
	pinMode(PIN_LED_WIFI, OUTPUT);
	pinMode(PIN_LED_LOOP, OUTPUT);
	pinMode(PIN_LED_ERROR, OUTPUT);
    Serial.begin(115200);
    setupSerialSaludo();
	configVar.load();
}


void reboot()
{
    Serial.println(F("<reboot()>"));
    ESP.restart();
}


void led(int pin_num, bool value)
{
	if (value)
	{
		digitalWrite(pin_num, HIGH);
	}
	else
	{
		digitalWrite(pin_num, LOW);
	}
}

void led_wifi(bool value) {led(PIN_LED_WIFI, value);}
void led_loop(bool value) {led(PIN_LED_LOOP, value);}
void led_error(bool value) {led(PIN_LED_ERROR, value);}

void loop()
{
	// Detección y procesamiento de debug input
	serialInputCommand.input();

	if (!updateOta)
	{
		// Control del LED Loop temporizado cada 1 segundo
		unsigned long currentTime = millis();
		if (currentTime - lastLedToggleTime >= 1000)  // 1000 ms = 1 segundo
		{
			lastLedToggleTime = currentTime;  // Actualiza el tiempo del último cambio
			led_loop_blink_state = !led_loop_blink_state;  // Alterna el estado del LED WiFi
			led_loop(led_loop_blink_state);  // Actualiza el LED
		}
		led_wifi(0);
		led_error(0);
		return;
	}

	Serial.println(F("Lanzando webserverAskUpdateOta ahora"));
	String firmwareURL = configVar.getStringByName("CONFIG_OTA_URL");

	// Verifica si firmwareURL está vacío
	if (firmwareURL.length() == 0) 
	{
		Serial.println("Error: La URL del firmware está vacía");
		updateOta = false;
		return;
	}

	// Verifica si firmwareURL no empieza con "http"
	if (!firmwareURL.startsWith("http")) 
	{
		Serial.println("Error: La URL del firmware no es válida");
		updateOta = false;
		return;
	}

	int httpCode = OTAUpdater::checkUrlExists(firmwareURL);
	if (httpCode != 200)
	{
		Serial.println("Error: http-code: " + String(httpCode));
		updateOta = false;
		return;
	}

	Serial.println("OTA: UPDATING " + firmwareURL);

	delay(50);
	led_wifi(1);
	led_loop(1);
	led_error(1);
	OTAUpdater::updateUrl(firmwareURL);
	updateOta = false;

}
