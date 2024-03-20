#include "tsCommon.h"
#include "tsTransportStream.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

//=============================================================================================================================================================================

const int packet_size = 188;

int main(int argc, char* argv[], char* envp[]){
    // TODO - open file -> DONE
    // TODO - check if file if opened -> DONE
    bool is_ok = true;
    FILE* file = std::fopen("../example_new.ts", "rb");

    if (!file) {
        is_ok = false;
        std::perror("File opening failed");
        return is_ok;
    }

    xTS_PacketHeader    TS_PacketHeader;

    int32_t TS_PacketId = 0;

    std::byte packet[packet_size];

    while (!std::feof(file)){
        // TODO - read from file -> DONE
        fread(packet, 1, packet_size, file);

        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse(/*TS_PacketBuffer*/ nullptr);

        printf("%010d ", TS_PacketId);
        TS_PacketHeader.Print();
        printf("\n");

        TS_PacketId++;
    }

    std::fclose(file);
    // TODO - close file -> DONE
    return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
