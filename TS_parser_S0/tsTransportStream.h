#pragma once
#include "tsCommon.h"
#include <string>
#include <cstdint>
/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS{
public:
  static constexpr uint32_t TS_PacketLength  = 188;
  static constexpr uint32_t TS_HeaderLength  = 4;

  static constexpr uint32_t PES_HeaderLength = 6;

  static constexpr uint32_t BaseClockFrequency_Hz         =    90000; //Hz
  static constexpr uint32_t ExtendedClockFrequency_Hz     = 27000000; //Hz
  static constexpr uint32_t BaseClockFrequency_kHz        =       90; //kHz
  static constexpr uint32_t ExtendedClockFrequency_kHz    =    27000; //kHz
  static constexpr uint32_t BaseToExtendedClockMultiplier =      300;
};

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================

class xTS_PacketHeader{
private:
    static constexpr int32_t syncByteMask = 0b11111111000000000000000000000000;
    static constexpr int32_t errorIndicatorMask = 0b00000000100000000000000000000000;
    static constexpr int32_t startIndicatorMask = 0b00000000010000000000000000000000;
    static constexpr int32_t transportPriorityMask = 0b00000000001000000000000000000000;
    static constexpr int32_t packetIdentifierMask = 0b00000000000111111111111100000000;
    static constexpr int32_t transportScramblingControlMask = 0b00000000000000000000000011000000;
    static constexpr int32_t adaptationFieldControlMask = 0b00000000000000000000000000110000;
    static constexpr int32_t continuityCounterMask = 0b00000000000000000000000000001111;

private:
    void parseSyncByte(int32_t header);
    void parseErrorIndicator(int32_t header);
    void parseStartIndicator(int32_t header);
    void parseTransportPriority(int32_t header);
    void parsePacketIdentifier(int32_t header);
    void parseTransportScramblingControl(int32_t header);
    void parseAdaptationFieldControl(int32_t header);
    void parseContinuityCounter(int32_t header);

public:
  enum class ePID : uint16_t{
    PAT  = 0x0000,
    CAT  = 0x0001,
    TSDT = 0x0002,
    IPMT = 0x0003,
    NIT  = 0x0010, //DVB specific PID
    SDT  = 0x0011, //DVB specific PID
    NuLL = 0x1FFF,
  };

protected:
    // header fields
    uint8_t  m_SB = {};
    uint8_t m_E = {};
    uint8_t m_S = {};
    uint8_t m_T = {};
    uint16_t m_PID = {};
    uint8_t m_TSC = {};
    uint8_t m_AFC = {};
    uint8_t m_CC = {};


public:
  void     Reset();
  int32_t  Parse(const uint8_t* Input);
  void     Print() const;

public:
  //direct acces to header fields value
  uint8_t  getSyncByte() const;
  uint8_t  getErrorIndicator() const;
  uint8_t  getStartIndicator() const;
  uint8_t  getTransportPriority() const;
  uint16_t  getPacketIdentifier() const;
  uint8_t  getTransportScramblingControl() const;
  uint8_t  getAdaptationFieldControl() const;
  uint8_t  getContinuityCounter() const;

public:
  //derrived informations
  bool  hasAdaptationField() const;
  bool  hasPayload() const;
};

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================

class xTS_AdaptationField {
private:
    static constexpr uint8_t m_DCMask = 0b10000000;
    static constexpr uint8_t m_RAMask = 0b01000000;
    static constexpr uint8_t m_SPMask = 0b00100000;
    static constexpr uint8_t m_PRMask = 0b00010000;
    static constexpr uint8_t m_ORMask = 0b00001000;
    static constexpr uint8_t m_SFMask = 0b00000100;
    static constexpr uint8_t m_TPMask = 0b00000010;
    static constexpr uint8_t m_EXMask = 0b00000001;

protected:
    //setup
    uint8_t m_AdaptationFieldControl = {};

    //mandatory fields
    uint8_t m_AdaptationFieldLength = {};
    uint8_t m_DC = {};
    uint8_t m_RA = {};
    uint8_t m_SP = {};
    uint8_t m_PR = {};
    uint8_t m_OR = {};
    uint8_t m_SF = {};
    uint8_t m_TP = {};
    uint8_t m_EX = {};

    //optional fields -PCR

public:
    void Reset();
    int32_t Parse(const uint8_t* PacketBuffer, uint8_t m_AdaptationFieldControl);
    void Print() const;

public:
    // mandatory fields
    uint8_t getAdaptationFieldLenght() const;
    //derived values
    //uint32_t getNumBytes() const {}
};

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xPES_PacketHeader
//=============================================================================================================================================================================

class xPES_PacketHeader
{
public:
    enum eStreamId : uint8_t
    {
        eStreamId_program_stream_map = 0xBC,
        eStreamId_padding_stream = 0xBE,
        eStreamId_private_stream_2 = 0xBF,
        eStreamId_ECM = 0xF0,
        eStreamId_EMM = 0xF1,
        eStreamId_program_stream_directory = 0xFF,
        eStreamId_DSMCC_stream = 0xF2,
        eStreamId_ITUT_H222_1_type_E = 0xF8,
    };
protected:
    //PES packet header
    uint32_t m_PacketStartCodePrefix;
    uint8_t m_StreamId;
    uint16_t m_PacketLength;
public:
    void Reset();
    int32_t Parse(const uint8_t* Input);
    void Print() const;
public:
    //PES packet header
    uint32_t getPacketStartCodePrefix() const;
    uint8_t getStreamId() const;
    uint16_t getPacketLength() const;
};


//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xPES_Assembler
//=============================================================================================================================================================================

class xPES_Assembler
{
public:
    enum class eResult : int32_t
    {
        UnexpectedPID = 1,
        StreamPackedLost,
        AssemblingStarted,
        AssemblingContinue,
        AssemblingFinished,
    };
protected:
    //setup
    int32_t m_PID;
    //buffer
    uint8_t* m_Buffer;
    uint32_t m_BufferSize;
    uint32_t m_DataOffset;
    //operation
    int8_t m_LastContinuityCounter = 0;
    bool m_Started;
    xPES_PacketHeader m_PESH;
public:
    void Init(int32_t PID);
    eResult AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField);
    void PrintPESH() const;
    uint8_t* getPacket();
    int32_t getNumPacketBytes() const;
protected:
    void xBufferReset();
    void xBufferAppend(const uint8_t* Data, int32_t Size);
};
