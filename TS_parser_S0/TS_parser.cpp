#include "tsCommon.h"
#include "tsTransportStream.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char* argv[], char* envp[]){
    
    FILE* file = std::fopen("../example_new.ts", "rb"); // open in binary mode

    if (!file){
        std::perror("File opening failed");
        return NOT_VALID;
    }

    xTS_PacketHeader TS_PacketHeader;

    int32_t TS_PacketId = 0;

    uint8_t packet[xTS :: TS_PacketLength];

    while (!std::feof(file)){
        if (fread(packet, 1, xTS::TS_PacketLength, file) == 188){ // check if number of bytes is correct
            TS_PacketHeader.Reset();
            TS_PacketHeader.Parse(packet);
            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
            printf("\n");
            TS_PacketId++;
        }
    }

    std::fclose(file);
    return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
