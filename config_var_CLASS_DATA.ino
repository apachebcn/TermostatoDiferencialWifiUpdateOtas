void configVar::load()
{
   
    _configVarId = 0;

    /*
        NETWORK
    */

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_WIFI_SSID");
    configVarMem[_configVarId].value = "";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 1;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_WIFI_PASSWORD");
    configVarMem[_configVarId].value = "";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 2;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_DINAMIC_ENABLE");
    configVarMem[_configVarId].value = "0";
    configVarMem[_configVarId].type = TYPE_BOOLEAN;
    configVarMem[_configVarId].sortPrintId = 3;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_DINAMIC_IP");
    configVarMem[_configVarId].value = "192.168.1.115";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 4;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_DINAMIC_MASK");
    configVarMem[_configVarId].value = "255.255.255.0";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 5;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_DINAMIC_GATEWAY");
    configVarMem[_configVarId].value = "192.168.1.1";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 6;
    _sumConfigVarId();

    configVarMem[_configVarId].groupName = F("Network");
    configVarMem[_configVarId].name = F("CONFIG_NETWORK_DINAMIC_DNS");
    configVarMem[_configVarId].value = "8.8.8.8";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 7;
    _sumConfigVarId();


     /*
    OTA
    */

    configVarMem[_configVarId].groupName = F("OTA");
    configVarMem[_configVarId].name = F("CONFIG_OTA_URL");
    configVarMem[_configVarId].value = "";
    configVarMem[_configVarId].type = TYPE_STRING;
    configVarMem[_configVarId].sortPrintId = 8;
    _sumConfigVarId();


    /*
    OTROS
    */

    lastConfigVarMem = _configVarId;
}
