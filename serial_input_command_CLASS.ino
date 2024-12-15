
void serialInputCommand::_inputNextCommand()
{
}

bool serialInputCommand::_inputGetNameValue(String& configName, String& configValue)
{
    int separatorIndex = _argumentSelected.indexOf('=');
    if (separatorIndex == -1)
    {
        Serial.println(F("Error: Falta simbolo igual"));
        return false;
    }
    configName = _argumentSelected.substring(0, separatorIndex);
    configValue = _argumentSelected.substring(separatorIndex + 1);
    if (!configName.length())
    {
        Serial.println(F("Error: configName NULL"));
        return false;
    }
    if (!configValue.length())
    {
        Serial.println(F("Error: configValue NULL"));
        return false;
    }
    return true;
}

bool serialInputCommand::_isNumeric(const String& str)
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (!isDigit(str.charAt(i)))
        {
            return false;
        }
    }
    return true;
}

bool serialInputCommand::_inputSetFilter(String configName, String &value)
{
	int configId = configVar.getConfigIdByName(configName);
	if (configVarMem[configId].type == TYPE_BOOLEAN)
	{
		if (value == "1" || value == "0") return true;
        value = "0";
        return false;
	}
	if (configVarMem[configId].type == TYPE_INTEGER)
	{
		if (_isNumeric(value)) return true;
        value = "0";
        return false;
	}
    return true;
}

void serialInputCommand::_inputSet()
{
    String configName = "";
    String configValue = "";
    if (!_inputGetNameValue(configName, configValue))
    {
        return;
    }
    if (configVar.getConfigIdByName(configName) < 0)
    {
        Serial.println(F("ESTA VARIABLE NO EXISTE"));
        return;
    }

    // Filtrar valor (para poder limitar valores según la finalidad de cada variable)
    if (!serialInputCommand::_inputSetFilter(configName, configValue))
    {
        Serial.println(F("VALOR INCORRECTO"));
        return;
    }

    // Asignar el valor a la variable correspondiente
    Serial.println("asignando a variable'" + configName + "' el valor '" + configValue + "'");
    configVar.setNameValue(configName, configValue);
}

bool serialInputCommand::_inputGetNameFromId()
{
    String configId = "";
    String configValue = "";
    if (!_inputGetNameValue(configId, configValue))
    {
        return false;
    }
    int configIdInt = configId.toInt();
	if (configIdInt > TOTAL_CONFIG_VAR_MEM) return false;
    String configName = String(configVarMem[configIdInt].name);
	_argumentSelected = configName+"="+configValue;
	return true;
}

void serialInputCommand::_getArgument()
{
	_argumentSelected = "";
    if (_inputUpper.startsWith(_commandSelected)) 
    {
        _argumentSelected = _input.substring(_commandSelected.length() + 1);  // Extrae la parte después del comando
        _argumentSelected.trim();  // Recorta espacios en blanco
        return;
    }
}

bool serialInputCommand::_inputConfigCommand(bool exist_command)
{

	_commandSelected = "REBOOT";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		Serial.println(F("REINICIANDO"));
		reboot();
		return true;
	}

	_commandSelected = "MEMFREE";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		Serial.println("Ram libre: " + String(ESP.getFreeHeap()) + " (" + String(ESP.getFreeHeap()/1024) + "Kb)");
		return true;
	}

	_commandSelected = "CONFIG LIST";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		configVar.printList(false, "");
		return true;
	}

	_commandSelected = "CONFIG LIST";
	if (_inputUpper.startsWith(_commandSelected))
	{
		if (exist_command) return true;
		configVar.printList(false, _argumentSelected);
		return true;
	}

	_commandSelected = "CONFIG SETID";
	if (_inputUpper.startsWith(_commandSelected))
	{
		if (exist_command) return true;
		if (!_inputGetNameFromId()) return false;
		_inputSet();
		return true;
	}

	_commandSelected = "CONFIG SET";
	if (_inputUpper.startsWith(_commandSelected))
	{
		if (exist_command) return true;
		_inputSet();
		return true;
	}

	_commandSelected = "WIFI STATUS";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		Serial.print("SSID conectado: "); Serial.println(WiFi.SSID());
		Serial.print("Canal: "); Serial.println(WiFi.channel());
		Serial.print("Intensidad: "); Serial.println(WiFi.RSSI());
		Serial.println(myWifi::wifi_info());
		return true;
	}

	_commandSelected = "WIFI CONNECT";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		myWifi::wifi_connect(
			WIFI_RECONNECT_TIMES,  // reintentos
			0  // fromSetup
		);
		return true;
	}

	_commandSelected = "WIFI DISCONNECT";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		myWifi::wifi_disconnect();
		return true;
	}

	_commandSelected = "WIFI SCAN";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		Serial.println(F("Escaneando redes WiFi cercanas..."));
		int numNetworks = WiFi.scanNetworks();
		if (numNetworks < 1) 
		{
			Serial.println(F("No se encontraron redes WiFi cercanas."));
		} 
		else 
		{
			Serial.print(F("Se encontraron "));
			Serial.print(numNetworks);
			Serial.println(F(" redes WiFi cercanas:"));
			for (int i = 0; i < numNetworks; ++i) 
			{
				Serial.print("  SSID: ");
				Serial.print(WiFi.SSID(i));
				Serial.print("  RSSI: ");
				Serial.println(WiFi.RSSI(i));
			}
		}            
		return true;
	}

	_commandSelected = "OTA UPDATE";
	if (_inputUpper == _commandSelected)
	{
		if (exist_command) return true;
		updateOta = true;
		return true;
	}

	_commandSelected = "";

    return false;
}

void serialInputCommand::input()
{
	_input = "";
    if (Serial.available() > 0) 
    {
        // Leer la entrada completa hasta que llegue un salto de línea
        _input = Serial.readStringUntil('\n');
        if (_input.length() > 200)
        {
            // Si la entrada excede los 100 caracteres, simplemente descartarla y salir de la función
            Serial.println("La entrada excede los 200 caracteres. Se descarta.");
            return;
        }

        _inputUpper = _input; // Copia la cadena original
        _inputUpper.toUpperCase(); // Convierte la copia a mayúsculas
        Serial.println();

        if (_inputUpper == "INFO" || _inputUpper == "HELP" || _inputUpper == "AYUDA")
        {
            _printHelp();
            return;
        }

        if (_inputConfigCommand(true))
        {
            Serial.println("Comando reconocido: " + _commandSelected);
            _getArgument();
            if (_argumentSelected.length())
            {
                Serial.println("Argumento: '" + _argumentSelected + "'");
            }
            _inputConfigCommand(false);
            _inputNextCommand();
            return;
        }
    }
}
