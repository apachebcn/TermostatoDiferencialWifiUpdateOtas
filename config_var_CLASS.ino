void configVar::_sumConfigVarId()
{
    _configVarId++;
    if (_configVarId > TOTAL_CONFIG_VAR_MEM)
    {
        Serial.println(F("_configVarId > TOTAL_CONFIG_VAR_MEM"));
    }
}

String configVar::getTypeString(int configVarType)
{
    switch(configVarType)
    {
        case TYPE_BOOLEAN:
            return "TYPE_BOOLEAN";
        case TYPE_INTEGER:
            return "TYPE_INTEGER";
        case TYPE_STRING:
            return "TYPE_STRING";
        default:
            return "?";
    }
}

int configVar::getConfigIdByName(const String configName)
{
    for (int configId = 0; configId < lastConfigVarMem; configId++)
    {
        if (configVarMem[configId].name == configName)
        {
            return configId;
        }
    }
    return -1;
}

char* configVar::getConfigNameById(int id)
{
    if (id > -1 && id < (lastConfigVarMem+1))
    {
        return (char*)configVarMem[id].name;
    }
    return "\0";
}

bool configVar::getBooleanByValue(const String value)
{
    if (value == "1")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool configVar::getBooleanByName(const String configName)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return false;

    String value = configVarMem[configId].value;
    return getBooleanByValue(value);
}

String configVar::getHtmlCheckedBooleanByName(const String configName)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return "";

    String value = configVarMem[configId].value;
    if (getBooleanByValue(value)) return "CHECKED";
    return "";
}

int configVar::getIntegerByValue(const String value)
{
    int intValue = value.toInt();
    if (value != String(intValue))
    {
        // Si la conversión falla, se devuelve un valor predeterminado
        return 0;
    }
    return intValue;
}

float configVar::getFloatByValue(const String value)
{
    float floatValue = value.toFloat();
    return floatValue;
}

int configVar::getIntegerByName(const String configName)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return 0;

    String value = configVarMem[configId].value;
    return getIntegerByValue(value);
}

float configVar::getFloatByName(const String configName)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return 0;

    String value = configVarMem[configId].value;
    return getFloatByValue(value);
}

String configVar::getStringByName(const String configName)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return "";
    return configVarMem[configId].value;
}

bool configVar::setNameValue(const String configName, const String valor)
{
    int configId = getConfigIdByName(configName);
    if (configId < 0) return false;
    configVarMem[configId].value = valor;
    return true;
}

void configVar::printList(bool mode, String coincidence)
{
    coincidence.trim();
    coincidence.toUpperCase();

    for (int configGroupId = 0; configGroupId < totalConfigVarGroupsNames; configGroupId++)
    {
        bool foundGroupName = false;

        // Estructura auxiliar para almacenar los índices y ordenarlos
        int indices[lastConfigVarMem];
        int count = 0;

        for (int configId = 0; configId < lastConfigVarMem; configId++)
        {
            if (coincidence.length())
            {
                String varname = configVarMem[configId].name;
                varname.toUpperCase();
                if (varname.indexOf(coincidence) < 0)
                {
                    continue;
                }
            }
            if (configVarMem[configId].groupName == configVarGroupsNames[configGroupId])
            {
                indices[count++] = configId;
            }
        }

        // Ordenar los índices según el campo "order"
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (configVarMem[indices[j]].sortPrintId > configVarMem[indices[j + 1]].sortPrintId)
                {
                    int temp = indices[j];
                    indices[j] = indices[j + 1];
                    indices[j + 1] = temp;
                }
            }
        }

        // Imprimir los valores ordenados dentro del grupo
        for (int i = 0; i < count; i++)
        {
            int configId = indices[i];
            if (!foundGroupName)
            {
                foundGroupName = true;
                Serial.println();
                Serial.println("================");
                Serial.println(configVarGroupsNames[configGroupId]);
                Serial.println("================");
            }
            if (mode == 1) printExtendedVar(configId); else printShortVar(configId);
        }
    }
}

void configVar::printExtendedVar(int configId)
{
    String debugMsg = "Id: " + String(configId);
    debugMsg += ", sortPrintId: " + String(configVarMem[configId].sortPrintId);
    debugMsg += ", type: " + getTypeString(configVarMem[configId].type);
    debugMsg += ", Name: " + String(configVarMem[configId].name);
    debugMsg += ", Value: " + configVarMem[configId].value;
    Serial.println(debugMsg);
}

void configVar::printShortVar(int configId)
{
    String debugMsg = String(configId) + "] ";
    debugMsg += String(configVarMem[configId].name) + " = '" + configVarMem[configId].value + "'";
    Serial.println(debugMsg);
}
