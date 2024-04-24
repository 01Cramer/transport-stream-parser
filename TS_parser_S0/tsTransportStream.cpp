#include "tsTransportStream.h"

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset(){
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t* Input){
	int32_t header = *(int32_t*)Input;
	header = xSwapBytes32(header);

	
	parseSyncByte(header);
	
	if (m_SB != 71) { // check if syncByte is correct
		return NOT_VALID;
	}

	parseErrorIndicator(header);

	parseStartIndicator(header);

	parseTransportPriority(header);

	parsePacketIdentifier(header);

	parseTransportScramblingControl(header);

	parseAdaptationFieldControl(header);

	parseContinuityCounter(header);

	return VALID;
}

void xTS_PacketHeader::parseSyncByte(int32_t header){
	int32_t syncByte = header & syncByteMask;
	syncByte = syncByte >> 24;
	syncByte = (int8_t)syncByte;
	m_SB = syncByte;
}

void xTS_PacketHeader::parseErrorIndicator(int32_t header){
	int32_t errorIndicator = header & errorIndicatorMask;
	errorIndicator = errorIndicator >> 23;
	errorIndicator = (int8_t)errorIndicator;
	m_E = errorIndicator;
}


void xTS_PacketHeader::parseStartIndicator(int32_t header) {
	int32_t startIndicator = header & startIndicatorMask;
	startIndicator = startIndicator >> 22;
	startIndicator = (int8_t)startIndicator;
	m_S = startIndicator;
}

void xTS_PacketHeader::parseTransportPriority(int32_t header) {
	int32_t transportPriority = header & transportPriorityMask;
	transportPriority = transportPriority >> 21;
	transportPriority = (int8_t)transportPriority;
	m_T = transportPriority;
}

void xTS_PacketHeader::parsePacketIdentifier(int32_t header) {
	int32_t packetIdentifier = header & packetIdentifierMask;
	packetIdentifier = packetIdentifier >> 8;
	packetIdentifier = (int16_t)packetIdentifier;
	m_PID = packetIdentifier;
}

void xTS_PacketHeader::parseTransportScramblingControl(int32_t header) {
	int32_t transportScramblingControl = header & transportScramblingControlMask;
	transportScramblingControl = transportScramblingControl >> 6;
	transportScramblingControl = (int8_t)transportScramblingControl;
	m_TSC = transportScramblingControl;
}

void xTS_PacketHeader::parseAdaptationFieldControl(int32_t header) {
	int32_t adaptationFieldControl = header & adaptationFieldControlMask;
	adaptationFieldControl = adaptationFieldControl >> 4;
	adaptationFieldControl = (int8_t)adaptationFieldControl;
	m_AFC = adaptationFieldControl;
}

void xTS_PacketHeader::parseContinuityCounter(int32_t header) {
	int32_t continuityCounter = header & continuityCounterMask;
	continuityCounter = (int8_t)continuityCounter;
	m_CC = continuityCounter;
}


/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const{ // ->DONE
	printf(" TS: ");
	printf("SB=");
	printf("%d", m_SB);
	printf(" E=");
	printf("%d", m_E);
	printf(" S=");
	printf("%d", m_S);
	printf(" P=");
	printf("%d", m_T);
	printf(" PID=");
	printf("%d", m_PID);
	printf(" TSC=");
	printf("%d", m_TSC);
	printf(" AF=");
	printf("%d", m_AFC);
	printf(" CC=");
	printf("%d", m_CC);
}

//=============================================================================================================================================================================
