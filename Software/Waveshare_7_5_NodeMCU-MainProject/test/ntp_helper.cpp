
#include "ntp_helper.h"


///   NOTE For now we will not expose these global variables to other users - lets see if we need to.
WiFiUDP ntpUDP;
// Create a NTP client
NTPClient timeClient(ntpUDP);


void CreateNTPClient()
{
    timeClient.begin(); // We only need to update the time once - so call this (rather than update it periodically)    
}