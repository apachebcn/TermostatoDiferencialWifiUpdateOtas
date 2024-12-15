#ifndef SERIAL_INPUT_COMMAND_CLASS_H
#define SERIAL_INPUT_COMMAND_CLASS_H

class serialInputCommand
{
    private:

        String _input;
        String _inputUpper;
        String _commandSelected;
        String _argumentSelected;

        bool _inputGetNameValue(String& configName, String& configValue);
        bool _isNumeric(const String& str);
        bool _inputSetFilter(String configName, String &value);
        bool _inputGetNameFromId();
        void _inputSet();
        void _inputSetId();
        void _getArgument();
        bool _inputConfigCommand(bool exist_command);
        void _printHelp();
        void _inputNextCommand();

    public:

        void input();
} serialInputCommand;

#endif
