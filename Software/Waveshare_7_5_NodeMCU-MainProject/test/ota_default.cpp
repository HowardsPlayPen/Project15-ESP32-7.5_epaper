#include <Arduino.h>

#include "ota_default.h"

unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}


// Allow any server to connect (i.e. turn off CORS)
// A web browser will send a pre-fetch of Http type "HTTP_OPTIONS" to check CORS status
void disable_cors(PsychicHttpServer& server)
{
  server.on("/*", HTTP_OPTIONS, [](PsychicRequest *request) {  

    PsychicResponse response(request);
    response.setCode(200);
    
    response.addHeader("Access-Control-Allow-Headers", "access-control-allow-origin");
    response.addHeader("Access-Control-Allow-Origin", "*");
    
    return response.send();
  });
}