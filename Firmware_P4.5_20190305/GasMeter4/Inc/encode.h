#ifndef __ENCODE_H
#define __ENCODE_H
#include "exparameter.h"
#include "ReportStatePacket.h"

void encodeCookingPacket(char **sendMeagess,CookingSessionReport_t *rPacket);
void encodeMeterStatusPacket(char **sendMeagess,reportStatePacket_t *rPacket);

#endif