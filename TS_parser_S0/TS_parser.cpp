#include "tsCommon.h"
#include "tsTransportStream.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char* argv[], char* envp[]){
    // TODO - open file -> DONE
    // TODO - check if file if opened -> DONE
    bool is_ok = true;
    FILE* file = std::fopen("../example_new.ts", "rb");

    if (!file){
        is_ok = false;
        std::perror("File opening failed");
        return is_ok;
    }

    xTS_PacketHeader    TS_PacketHeader;

    int32_t TS_PacketId = 0;

    std::uint8_t packet[xTS :: TS_PacketLength];

    while (!std::feof(file)){
        // TODO - read from file -> DONE
        fread(packet, 1, xTS::TS_PacketLength, file);

        TS_PacketHeader.Reset();
        TS_PacketHeader.Parse(packet);

        if (TS_PacketId <= 33) {
            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
            printf("\n");
        }

        TS_PacketId++;
    }

    std::fclose(file);
    // TODO - close file -> DONE
    return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
