// We allow comments to be in the JSON (they are ignored)
#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>

/* Includes ------------------------------------------------------------------*/
#include "ePaper/DEV_Config.h"
#include "ePaper/EPD.h"
#include "ePaper/GUI_Paint.h"
#include "ePaper/imagedata.h"
#include "ePaper/ImageData.h"
#include <stdlib.h>
#include "time.h"

#include "ssdp_helper.hpp"

#include <PsychicHttp.h>
#include <ElegantOTA.h>
#include <WiFi.h>

#include "json_settings.hpp"
#include "ota_default.hpp"
#include "ntp_helper.hpp"

// Define the REST server instance
PsychicWebSocketHandler websocketHandler;
PsychicEventSource eventSource;
PsychicHttpServer server;

//Create a new image cache named IMAGE_BW and fill it with white
UBYTE *BlackImage, *RYImage;

typedef struct {
  String timeString;
  String dateString;
  String ipAddress;
} ApplicationState;

ApplicationState myState; 

// Simple handler for index page 
esp_err_t get_index_html(PsychicRequest *request)
{
  PsychicResponse response(request);
  response.setCode(200);
  response.setContentType("text/html");
  // TODO add a sensible home page here
  //response.setContent((const uint8_t*)index_html,  (int) (index_html_end - index_html));  
  return response.send();
}


extern void BlackDisplay(const UBYTE *blackimage);


void drawFunction()
{
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    
    //Paint_DrawString_EN(720, 15,   myState.dateString.c_str(), &Font24, WHITE, BLACK);
    Paint_DrawString_EN(300, 200, myState.timeString.c_str(), &Font24, WHITE, BLACK);    
    Paint_DrawString_EN(1,   460, myState.ipAddress.c_str(), &Font16, WHITE, BLACK);
    Paint_DrawRectangle(680, 1, 800, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(690, 15,   "Waveshare 7.5'", &Font12, WHITE, BLACK);

    
    BlackDisplay(BlackImage);
}

/* Entry point ----------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);
  printf("EPD_7IN5B_V2_test Demo\r\n");

  JsonDocument doc;
  if(!LoadJsonSettings(doc))
  {
      /// TODO HANDLE THIS ERROR - e.g. FLASH THE LED
  }

  JsonObject root = doc.as<JsonObject>();

  if(!LoadDefaultParameters(root))
  {
    //// TODO HANDLE THIS ERROR
  }
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connected");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  myState.ipAddress = WiFi.localIP().toString();

  CreateNTPClient(); // Sync our time using NTP  

  timeClient.update(); // This syncs the time - so far we do not need to update this regularly.

  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_7IN5B_V2_Init();
  Serial.println("Init");
  EPD_7IN5B_V2_Clear();
  DEV_Delay_ms(500);
  Serial.println("Clear");


  UWORD Imagesize = ((EPD_7IN5B_V2_WIDTH % 8 == 0) ? (EPD_7IN5B_V2_WIDTH / 8 ) : (EPD_7IN5B_V2_WIDTH / 8 + 1)) * EPD_7IN5B_V2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while(1);
  }
  if ((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\r\n");
    while(1);
  }
  printf("NewImage:BlackImage and RYImage\r\n");
  Paint_NewImage(BlackImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);
  Paint_NewImage(RYImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);

  // Set Authentication Credentials
   ElegantOTA.setAuth(ota_user.c_str(), ota_password.c_str());
  // start server
  server.listen(80); // MUST call listen() before registering any urls using .on()

  server.on("/", HTTP_GET, get_index_html);

  ssdp_helper params{"/update", "Waveshare 7.5 ePaper Display","ESP32","ePaper smart display","https://****"};
  
  setup_ssdp(params,server);

  // The below function registers a handler with the Web server to generically handle HTTP_OPTIONS and add the flags that we are not worried about CORS
  disable_cors(server); // CORS is pointless for an IOT device here

  // Note: ElegantOTA listens on the url "/update"
  ElegantOTA.begin(&server);    // Start ElegantOTA

  //ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  // NOTE The below is debug code - it seems that getLocalTime DOESNT work so I will not bother trying to get the date
  // myState.dateString = "GOOD";

  // struct tm timeinfo;
  // if(!getLocalTime(&timeinfo)){
  //   myState.dateString = "FAIL";
  // }

  myState.timeString = timeClient.getFormattedTime();   

  drawFunction();
}

unsigned long previousMillis = 0;
const long interval = 60000; // interval at which to read (milliseconds)


/* The main loop -------------------------------------------------------------*/
void loop()
{
  ElegantOTA.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;  
      
    // Get the current time
    myState.timeString = timeClient.getFormattedTime();   

    drawFunction();
  }
}
