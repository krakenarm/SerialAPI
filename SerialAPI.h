#ifndef SERIALAPI_H
#define SERIALAPI_H

#include "Arduino.h"


class SerialAPI
{
    public:

        SerialAPI(byte protocolV);
        SerialAPI();
        typedef char Command[2];
        typedef char ByteParam[3];

        void dumpBuffers();
        static byte readSerialTag(char* buffer, byte bufferSize);
        static void readCommand(char* buffer, byte bufferLength, Command commandBuffer);
        void readCommand(Command commandBuffer);
        static bool isCommand(char* buffer, byte length);
        static void waitForCommand(char* buffer, byte length);
        void waitForCommand();
        bool tryHandShake();
        void answerHandShake();
        bool tryGoodBye();
        void answerGoodBye();
        void resetBuffers(bool clearInput, bool clearOutput);
        bool listen();
        static bool commandEquals(Command com1, Command com2);
        static bool commandEquals(Command com, byte com_ID);
        static void clearBuffer(char* buffer, byte size);
        static void check(char* buffer, byte length);
        static void setCommand(Command com, byte com_ID);
        void setCommand(byte com_ID);
        static void createCommand(char* buffer, byte com_ID);
        static void send(char* buffer);
        void send();
        static char TAG_START;
        static char TAG_END;
        void sendCommand(byte com_ID);
        bool tryProtocolExchange();
        void answerProtocolExchange();
        bool isProtocolVersionSupported(byte v);
        static void convertToByteParam(byte b, ByteParam byteParam);
        void addParam(byte b, byte nth);
        void addParams(byte* params, byte nParams);
        byte countParams();
        void readParams(byte* params, byte nParams);
        void readByteParam(byte nth, ByteParam byteParam);
        static char COMMAND_PARAM_SEPARATOR;
        static byte COMMAND_LENGTH;
        static char COMMAND_END_MARKER;
        static byte BYTE_PARAM_LENGTH;

        static byte COM_HI;
        static byte COM_BYE;
        static byte COM_OK;
        static byte COM_PROTOCOL_VERSION;


        static Command COMMANDS[10];

        static void setup();


protected:
        static const byte BUFFER_SIZE;

        bool waitingForInput;
        byte protocolVersion;

    private:
        char input[32];
        char output[32];
        static byte getStringLength(byte* buffer, byte size);
};

#endif
