#ifndef OTA_H
    #define OTA_H


    class OTAUpdater
    {

        private:

            static void _update(const String &firmwareURL);

        public:

            static int checkUrlExists(const String &url);
            static void updateUrl(String &firmwareURL);

    };

#endif
