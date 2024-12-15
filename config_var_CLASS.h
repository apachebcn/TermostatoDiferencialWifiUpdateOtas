#ifndef CONFIG_VAR_CLASS_H
    #define CONFIG_VAR_CLASS_H

    #define TYPE_BOOLEAN 0
    #define TYPE_INTEGER 1
    #define TYPE_STRING 2

    #define TOTAL_CONFIG_VAR_MEM 9

    const __FlashStringHelper* configVarGroupsNames[] = {
        F("Network"),
        F("OTA")
    };

    struct configVarMemStruct
    {
        const __FlashStringHelper* name;
        const __FlashStringHelper* groupName;
        String value = "";
        byte type = 0;  // 0=BOOL, 1=INT, 2=STRING
        byte sortPrintId = 0;
    };

    const int totalConfigVarGroupsNames = sizeof(configVarGroupsNames) / sizeof(configVarGroupsNames[0]);
    configVarMemStruct configVarMem[TOTAL_CONFIG_VAR_MEM];

    class configVar
    {

        private:

            int _configVarId = 0;
            void _sumConfigVarId();    

        public:

            int lastConfigVarMem = 0;

            String getTypeString(int configVarType);
            bool getBooleanByValue(const String value);
            int getIntegerByValue(const String value);
            float getFloatByValue(const String value);
            int getConfigIdByName(const String configName);
            char* getConfigNameById(int id);
            bool getBooleanByName(const String configName);
            String getHtmlCheckedBooleanByName(const String configName);
            int getIntegerByName(const String configName);
            float getFloatByName(const String configName);
            String getStringByName(const String configName);
            bool setNameValue(const String configName, const String valor);
            void printList(bool mode, String coincidence);
            void printExtendedVar(int configId);
            void printShortVar(int configId);
            void load();
    } configVar;

#endif
