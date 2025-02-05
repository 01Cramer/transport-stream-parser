#include "tsCommon.h"
#include "tsTransportStream.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char* argv[], char* envp[]){
    
    FILE* transportStreamFile = std::fopen("../example_new.ts", "rb"); // open in binary mode
    FILE* audioFile = std :: fopen("../PID136.mp2", "wb");

    if (!transportStreamFile || !audioFile){
        std::perror("File opening failed");
        return NOT_VALID;
    }

    xTS_PacketHeader TS_PacketHeader;
    xTS_AdaptationField TS_PacketAdaptationField;
    
    xPES_PacketHeader PES_PacketHeader;
    xPES_Assembler PES_Assembler;

    int32_t TS_PacketId = 0;
    uint8_t TS_PacketBuffer[xTS :: TS_PacketLength];

    while (!std::feof(transportStreamFile)){
        if (fread(TS_PacketBuffer, 1, xTS::TS_PacketLength, transportStreamFile) == xTS::TS_PacketLength){ // check if number of bytes is correct
            TS_PacketHeader.Reset();
            TS_PacketHeader.Parse(TS_PacketBuffer); 
            TS_PacketAdaptationField.Reset();
            if (TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPacketIdentifier() == 136){
                if (TS_PacketHeader.hasAdaptationField()){
                    TS_PacketAdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.getAdaptationFieldControl());
                }
                printf("%010d ", TS_PacketId);
                TS_PacketHeader.Print();
                if (TS_PacketHeader.hasAdaptationField()) { 
                    TS_PacketAdaptationField.Print(); 
                }
                PES_Assembler.Init(TS_PacketHeader.getPacketIdentifier());
                xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_PacketAdaptationField);
                switch (Result)
                {
                case xPES_Assembler::eResult::StreamPackedLost: printf(" PcktLost "); break;
                case xPES_Assembler::eResult::AssemblingStarted: printf(" Started "); PES_Assembler.PrintPESH(); break;
                case xPES_Assembler::eResult::AssemblingContinue: printf(" Continue "); break;
                case xPES_Assembler::eResult::AssemblingFinished: 
                    printf(" Finished "); 
                    printf("PES: PacketLen=%d HeadLen=%d DataLen=%d", PES_Assembler.getNumPacketBytes(), PES_Assembler.m_PESH.getPESHeaderLength(), PES_Assembler.getNumPacketBytes() - PES_Assembler.m_PESH.getPESHeaderLength()); 
                    std::fwrite(PES_Assembler.getPacket() + PES_Assembler.m_PESH.getPESHeaderLength(), 1, PES_Assembler.getNumPacketBytes() - PES_Assembler.m_PESH.getPESHeaderLength(), audioFile);
                    PES_Assembler.xBufferReset(); 
                    break;
                default: break;
                }
                printf("\n");
                TS_PacketId++;
            }
        }
        else {
            break;
        }
    }

    std::fclose(transportStreamFile);
    std::fclose(audioFile);
    return EXIT_SUCCESS;
}
//=============================================================================================================================================================================
