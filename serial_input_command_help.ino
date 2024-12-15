void serialInputCommand::_printHelp()
{
    Serial.println(F("AYUDA/HELP/INFO:"));
    Serial.println();

    Serial.println();
    Serial.println(F("Para manejar las variables de configuración:"));
    Serial.println(F("- CONFIG SET {variable}={valor}"));
    Serial.println(F("      Asigna un valor a una variable-config"));
    Serial.println(F("- CONFIG SETID {variable_id}={valor}"));
    Serial.println(F("      Asigna un valor a una variable-config mencionandola por su id"));
    Serial.println(F("- CONFIG LIST"));
    Serial.println(F("      Lista todas las variables-config"));
    Serial.println(F("- CONFIG LIST palabra"));
    Serial.println(F("      Lista todas las variables-config con coincidencia {palabra}"));
    Serial.println();

    Serial.println(F("Forzar loop:"));
    Serial.println(F("- LOOP"));
    Serial.println();
    Serial.println(F("- WIFI STATUS"));
    Serial.println(F("      Muestra el estado del wifi y configuración de la red"));
    Serial.println(F("- WIFI SCAN"));
    Serial.println(F("      Escanea redes wifis cercanas"));
    Serial.println(F("- WIFI CONNECT"));
    Serial.println(F("      Intenta conectar al wifi configurado"));
    Serial.println(F("- WIFI DISCONNECT"));
    Serial.println(F("      Desconecta la conexión wifi"));
    Serial.println();

    Serial.println(F("Print info:"));

    Serial.println(F("- OTA UPDATE"));
    Serial.println(F("      Descarga y actualiza la última versión del firmware del servidor Ota"));
    Serial.println();

    Serial.println(F("- MEMFREE:"));
    Serial.println(F("      Muestra memoria libre en bytes"));
    Serial.println(F("- REBOOT:"));
    Serial.println(F("      Reinicia la placa. (es como apagar y encender)"));
    Serial.println();
}
