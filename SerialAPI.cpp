#include "Arduino.h"
#include "SerialAPI.h"

SerialAPI::SerialAPI(byte protocolV) {
    protocolVersion = protocolV;
    waitingForInput = false;
}
SerialAPI::SerialAPI() {
    SerialAPI(0);
}

void SerialAPI::addParam(byte b, byte nth) {
    ByteParam byteParam;
    convertToByteParam(b, byteParam);
    // COMMAND_LENGTH+COMMAND_END_MARKER+(nth*(BYTE_PARAM_LENGTH+COMMAND_PARAM_SEPARATOR))
    byte start = COMMAND_LENGTH+1+(nth*(BYTE_PARAM_LENGTH+1));
    byte i;
    for (i = 0 ; i < BYTE_PARAM_LENGTH; i++) {
        output[start+i] = byteParam[i];
    }
    output[start+i+1] = COMMAND_PARAM_SEPARATOR;
    output[start+i+2] = '\0';
}
void SerialAPI::addParams(byte *params, byte nParams) {
    for (byte i=0; i<nParams; i++) {
        addParam(params[i], i);
    }
}
byte SerialAPI::countParams() {
    byte counter = 0;
    byte i;
    for (i=0; i<BUFFER_SIZE; i++) {
        if (input[i] == COMMAND_PARAM_SEPARATOR)
            counter++;
    }
    return counter;
}
void SerialAPI::readByteParam(byte nth, ByteParam byteParam) {

}
void SerialAPI::readParams(byte *params, byte nParams) {
    byte i;
    for ( i=0; i<nParams; i++) {
        ByteParam byteParam;

    }
}
bool SerialAPI::tryProtocolExchange() {

}
bool SerialAPI::isProtocolVersionSupported(byte v) {
    return (v<1);
}
void SerialAPI::convertToByteParam(byte b, ByteParam byteParam) {
    byte v;
    for (byte i = 0; i < 3; ++i) {
        //TODO: finish
    }
}
void SerialAPI::answerProtocolExchange() {

}
void SerialAPI::send() {
    waitingForInput=false;
    send(output);
    clearBuffer(output, BUFFER_SIZE);
}
void SerialAPI::setCommand(byte com_ID) {
    memcpy(output, COMMANDS[com_ID], COMMAND_LENGTH);
    output[COMMAND_LENGTH] = COMMAND_END_MARKER;
    output[COMMAND_LENGTH+1] = '\0';
}
void SerialAPI::waitForCommand() {
    waitingForInput=true;
    bool success = false;
    byte length;
    while (!success) {
        length = readSerialTag(input, BUFFER_SIZE);
        if (length>0) {
            if (isCommand(input, BUFFER_SIZE)) {
                success = true;
            }
        }
    }
}
void SerialAPI::readCommand(char *commandBuffer) {
    readCommand(input, BUFFER_SIZE, commandBuffer);
}

void SerialAPI::sendCommand(byte com_ID) {
    setCommand(com_ID);
    send();
}
bool SerialAPI::tryHandShake() {
    setCommand(COM_HI);
    send();
    waitForCommand();
    Command com;
    readCommand(com);
    if (commandEquals(com, COM_HI)) {
        return true;
    }
    return false;
}
bool SerialAPI::tryGoodBye() {
    setCommand(COM_BYE);
    send();
    waitForCommand();
    Command com;
    readCommand(com);
    if (commandEquals(com, COM_BYE)) {
        return true;
    }
    return false;
}
void SerialAPI::answerGoodBye() {
    setCommand(COM_BYE);
    send();
}
void SerialAPI::resetBuffers(bool clearInput, bool clearOutput) {
    if (clearInput) {
        clearBuffer(input, BUFFER_SIZE);
    }
    if (clearOutput) {
        clearBuffer(output, BUFFER_SIZE);
    }
}
bool SerialAPI::listen() {
    byte length = readSerialTag(input, BUFFER_SIZE);
    if (length>0) {
        if (isCommand(input, BUFFER_SIZE)) {
            Command com;
            readCommand(com);
            char test[3];
            memcpy(test, com, 2);
            test[2]='\0';
            if (commandEquals(com, COM_HI))
                answerHandShake();
            else if(commandEquals(com, COM_BYE))
                answerGoodBye();

        }
        return true;
    }
    return false;
}
void SerialAPI::answerHandShake() {
    setCommand(COM_HI);
    send();
}
byte SerialAPI::getStringLength(byte* buffer, byte size) {
    for(byte i=0; i<size; i++) {
        if (buffer[i]== '\0') {
            return i;
        }
    }
    return size;
}
void SerialAPI::setup() {
    Serial.println(COMMANDS[COM_HI]);
}

void SerialAPI::readCommand(char *buffer, byte bufferLength, char *commandBuffer) {
    for (byte i = 0; i < COMMAND_LENGTH; ++i) {
        commandBuffer[i] = buffer[i];
    }
}
bool SerialAPI::isCommand(char *buffer, byte length) {
    return (buffer[COMMAND_LENGTH]== COMMAND_END_MARKER);
}
bool SerialAPI::commandEquals(Command com1, char* com2) {
    for (byte i = 0; i <COMMAND_LENGTH; ++i) {
        if (com1[i] != com2[i]) {
            return false;
        }
    }
    return true;
}

void SerialAPI::createCommand(char *buffer, byte com_ID) {
    memcpy(buffer, COMMANDS[com_ID], COMMAND_LENGTH);
    buffer[COMMAND_LENGTH] = COMMAND_END_MARKER;
    buffer[COMMAND_LENGTH+1] = '\0';
}
void SerialAPI::send(char *buffer) {
    Serial.print(TAG_START);
    Serial.print(buffer);
    Serial.println(TAG_END);
}
bool SerialAPI::commandEquals(Command com, byte com_ID) {
    return commandEquals(com, COMMANDS[com_ID]);
}
void SerialAPI::setCommand(Command com, byte com_ID) {
    memcpy(com, COMMANDS[com_ID], COMMAND_LENGTH);
}
byte SerialAPI::readSerialTag(char *buffer, byte bufferSize) {
    if (Serial.available()<1) {
        return 0;
    }
    char ch;
    byte bufferPosition = 0;
    bool endReached = false;
    bool hasStarted = false;
    while(!endReached) {
        while (Serial.available() > 0) {
            ch = Serial.read();
            // if startMarker found
            if (ch == TAG_START) {
                hasStarted = true;
            }
            else {
                // if endMarker found
                if (ch == TAG_END) {
                    endReached = true;
                    // mark the last char as string end
                    buffer[bufferPosition] = '\0';
                    bufferPosition++;
                }
                else {
                    // if data inside markers
                    if (hasStarted) {
                        buffer[bufferPosition] = ch;
                        bufferPosition++;
                    }
                    else {
                        return 0;
                    }
                }
            }
        }
    }
    return SerialAPI::getStringLength(buffer, bufferSize);
}
void SerialAPI::dumpBuffers() {
    Serial.print("input: ");Serial.println(input);
    Serial.print("output: ");Serial.println(output);
}
void SerialAPI::waitForCommand(char* buffer, byte length) {
    byte l;
    bool success = false;
    while (!success) {
        l = readSerialTag(buffer, length);
        if (l>0) {
            if (isCommand(buffer, l)) {
                success = true;
            }
        }
    }
}

void SerialAPI::check(char *buffer, byte length) {
    if (isCommand(buffer, length)) {
        Serial.println("command found");
        Command com;
        readCommand(buffer, length, com);
        if (commandEquals(com, COM_HI)) {
            Serial.println("says HI!");
            char output[64];
            createCommand(output, COM_HI);
            send(output);
            char input[64];
            waitForCommand(input, 64);
            Serial.print("recieved answere: "); Serial.println(input);
        }
        else if (commandEquals(com, COM_BYE)) {
            Serial.println("says BYE!");
        }
    }
}
void SerialAPI::clearBuffer(char *buffer, byte size) {
    for (byte i = 0; i < size; ++i) {
        buffer[i] = '\0';
    }
}

char SerialAPI::TAG_START = '<';
char SerialAPI::TAG_END = '>';

char COMMAND_PARAM_SEPARATOR = ';';

byte SerialAPI::COMMAND_LENGTH = 2;
byte SerialAPI::BYTE_PARAM_LENGTH = 3;
char SerialAPI::COMMAND_END_MARKER = '!';



byte SerialAPI::COM_HI = 0;
byte SerialAPI::COM_BYE = 1;
byte SerialAPI::COM_OK = 2;
byte SerialAPI::COM_PROTOCOL_VERSION = 3;

const byte SerialAPI::BUFFER_SIZE = 32;

SerialAPI::Command SerialAPI::COMMANDS[10] =
    {
        {'H', 'I'},
        {'B', 'Y'},
        {'O', 'K'},
        {'P', 'V'}
    }
;
