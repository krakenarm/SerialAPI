#ifndef SERIALAPI_H
#define SERIALAPI_H

#include "Arduino.h"


class SerialAPI
{
    public:

    // constructors
        SerialAPI(byte protocolV);
        SerialAPI();

    // typedefs

        typedef int (* FunctionPointerIntInt)(int);
        typedef char Command[2];
        static byte COMMAND_LENGTH;

        typedef char ByteParam[3];
        static byte BYTE_PARAM_LENGTH;


    // static
        // read/write
        static byte readSerialTag(char* buffer, byte bufferSize);
        static void send(char* buffer);

        // Command
        static bool isCommand(char* buffer, byte length);
        static bool commandEquals(Command com1, Command com2);
        static bool commandEquals(Command com, byte com_ID);
        static void readCommand(char* buffer, byte bufferLength, Command commandBuffer);
        static void waitForCommand(char* buffer, byte length);
        static void setCommand(Command com, byte com_ID);
        static void createCommand(char* buffer, byte com_ID);

        // variables
        static byte COM_PROTOCOL_VERSION;

        static char TAG_START;
        static char TAG_END;

        static char COMMAND_PARAM_SEPARATOR;
        static char COMMAND_END_MARKER;

        static Command COMMANDS[10];
        static byte COM_HI;
        static byte COM_BYE;
        static byte COM_OK;

    //helpers
        static void clearBuffer(char* buffer, byte size);
        static void convertToByteParam(byte b, ByteParam byteParam);
        static byte convertFromByteParam(ByteParam byteParam);

    // Instance
        // Commands
        void readCommand(Command commandBuffer);
        void setCommand(byte com_ID);
        void waitForCommand();
        void addParam(byte b, byte nth);
        void addParams(byte* params, byte nParams);
        byte countParams();
        void readParams(byte* params, byte nParams);
        void readByteParam(byte nth, ByteParam byteParam);


        // communication
        bool listen();
        void send();
        void sendCommand(byte com_ID);

        bool tryHandShake();
        void answerHandShake();

        bool tryGoodBye();
        void answerGoodBye();

        bool tryProtocolExchange();
        void answerProtocolExchange();

        void answerOK();

        void recieveCommandsUntilBye();

        // helpers
        void resetBuffers(bool clearInput, bool clearOutput);
        void dumpBuffers();
        bool isProtocolVersionSupported(byte v);

        int callIntInt(FunctionPointerIntInt* functionsArray, byte index, int param);
    protected:

    // static
        // variables
        static const byte BUFFER_SIZE;

    // Instance
        // variables
        bool waitingForInput;
        byte protocolVersion;

    private:

    // static
        // helpers
        static byte getStringLength(byte* buffer, byte size);

    // Instance

        // variables
        char input[32];     // length should match BUFFER_SIZE
        char output[32];    // length should match BUFFER_SIZE
};

#endif
