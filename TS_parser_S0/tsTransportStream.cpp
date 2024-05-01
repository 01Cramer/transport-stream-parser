#include "tsTransportStream.h"

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================

/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset(){
	uint8_t  m_SB = 0;
	uint8_t m_E = 0;
	uint8_t m_S = 0;
	uint8_t m_T = 0;
	uint16_t m_PID = 0;
	uint8_t m_TSC = 0;
	uint8_t m_AFC = 0;
	uint8_t m_CC = 0;
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
	if (m_SB != 71){ // check if syncByte is correct
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


void xTS_PacketHeader::parseStartIndicator(int32_t header){
	int32_t startIndicator = header & startIndicatorMask;
	startIndicator = startIndicator >> 22;
	startIndicator = (int8_t)startIndicator;
	m_S = startIndicator;
}

void xTS_PacketHeader::parseTransportPriority(int32_t header){
	int32_t transportPriority = header & transportPriorityMask;
	transportPriority = transportPriority >> 21;
	transportPriority = (int8_t)transportPriority;
	m_T = transportPriority;
}

void xTS_PacketHeader::parsePacketIdentifier(int32_t header){
	int32_t packetIdentifier = header & packetIdentifierMask;
	packetIdentifier = packetIdentifier >> 8;
	packetIdentifier = (int16_t)packetIdentifier;
	m_PID = packetIdentifier;
}

void xTS_PacketHeader::parseTransportScramblingControl(int32_t header){
	int32_t transportScramblingControl = header & transportScramblingControlMask;
	transportScramblingControl = transportScramblingControl >> 6;
	transportScramblingControl = (int8_t)transportScramblingControl;
	m_TSC = transportScramblingControl;
}

void xTS_PacketHeader::parseAdaptationFieldControl(int32_t header){
	int32_t adaptationFieldControl = header & adaptationFieldControlMask;
	adaptationFieldControl = adaptationFieldControl >> 4;
	adaptationFieldControl = (int8_t)adaptationFieldControl;
	m_AFC = adaptationFieldControl;
}

void xTS_PacketHeader::parseContinuityCounter(int32_t header){
	int32_t continuityCounter = header & continuityCounterMask;
	continuityCounter = (int8_t)continuityCounter;
	m_CC = continuityCounter;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const{
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

uint8_t  xTS_PacketHeader :: getSyncByte() const { return m_SB; }
uint8_t  xTS_PacketHeader :: getErrorIndicator() const { return m_E; }
uint8_t  xTS_PacketHeader :: getStartIndicator() const { return m_S; }
uint8_t  xTS_PacketHeader :: getTransportPriority() const { return m_T; }
uint16_t xTS_PacketHeader :: getPacketIdentifier() const { return m_PID; }
uint8_t  xTS_PacketHeader :: getTransportScramblingControl() const { return m_TSC; }
uint8_t  xTS_PacketHeader :: getAdaptationFieldControl() const { return m_AFC; }
uint8_t  xTS_PacketHeader :: getContinuityCounter() const { return m_CC; }

bool  xTS_PacketHeader :: hasAdaptationField() const{
	if (m_AFC == 2 || m_AFC == 3){
		return true;
	}
	else {
		return false;
	}
}

bool  xTS_PacketHeader :: hasPayload() const{
	if (m_AFC == 1 || m_AFC == 3) {
		return true;
	}
	else {
		return false;
	}
}

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================

void xTS_AdaptationField::Reset(){
	uint8_t m_AdaptationFieldLength = 0;
	uint8_t m_DC = 0;
	uint8_t m_RA = 0;
	uint8_t m_SP = 0;
	uint8_t m_PR = 0;
	uint8_t m_OR = 0;
	uint8_t m_SF = 0;
	uint8_t m_TP = 0;
	uint8_t m_EX = 0;
}

/**
@brief Parse adaptation field
@param PacketBuffer is pointer to buffer containing TS packet
@param AdaptationFieldControl is value of Adaptation Field Control field of
corresponding TS packet header
@return Number of parsed bytes (length of AF or -1 on failure)
*/
int32_t xTS_AdaptationField::Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl){
	for (int i = 0; i < xTS :: TS_HeaderLength; i++) { // Skipping header
		PacketBuffer++;
	}

	m_AdaptationFieldLength = *PacketBuffer;
	PacketBuffer++;
	
	m_DC = (*PacketBuffer & m_DCMask) >> 7;
	m_RA = (*PacketBuffer & m_RAMask) >> 6;
	m_SP = (*PacketBuffer & m_SPMask) >> 5;
	m_PR = (*PacketBuffer & m_PRMask) >> 4;
	m_OR = (*PacketBuffer & m_ORMask) >> 3;
	m_SF = (*PacketBuffer & m_SFMask) >> 2;
	m_TP = (*PacketBuffer & m_TPMask) >> 1;
	m_EX = (*PacketBuffer & m_EXMask);

	return m_AdaptationFieldLength;

}

void xTS_AdaptationField::Print() const{
	printf(" AF: ");
	printf(" L = ");
	printf("%d", m_AdaptationFieldLength);
	printf(" DC=");
	printf("%d", m_DC);
	printf(" RA=");
	printf("%d", m_RA);
	printf(" SP=");
	printf("%d", m_SP);
	printf(" PR=");
	printf("%d", m_PR);
	printf(" OR=");
	printf("%d", m_OR);
	printf(" SF=");
	printf("%d", m_SF);
	printf(" TP=");
	printf("%d", m_TP);
	printf(" EX=");
	printf("%d", m_EX);
}

uint8_t xTS_AdaptationField :: getAdaptationFieldLenght() const {
	return m_AdaptationFieldLength;
}
//=============================================================================================================================================================================
