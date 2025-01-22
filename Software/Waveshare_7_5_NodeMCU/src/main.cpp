/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "ImageData.h"
#include <stdlib.h>

#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

#include <Preferences.h>

// #include "soc/soc.h"
// #include "soc/rtc_cntl_reg.h"

AsyncWebServer server(80);

// This is the ESP32 library that gives us access to the onboard EEPROM / Flash
Preferences preferences;


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


/* Entry point ----------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);
  printf("EPD_7IN5B_V2_test Demo\r\n");

  preferences.begin("credentials", false); 
  Serial.println("Reading from onboard Flash");

  // Pull wifi details from the onboard flash memory
  String ssid = preferences.getString("ssid", ""); 
  String password = preferences.getString("password", "");

  Serial.println(ssid);

  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
  // delay(3000);
  // Serial.println("HOLD ON");
  
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

  // delay(2000);
  // Serial.println("ePAPER");
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); //enable brownout detector

  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_7IN5B_V2_Init();
  Serial.println("Init");
  EPD_7IN5B_V2_Clear();
  DEV_Delay_ms(500);
  Serial.println("Clear");



  // We store the username and password for the OTA update side in Flash - i.e. so that when you wish to update the firmware for this board you need to have knowledge of the board specific OTA password
  String elegantUser = "***";//preferences.getString("elegantUser", ""); 
  String elegantPassword = "***";//preferences.getString("elegantPassword", ""); 

  Serial.println("OTA user: " + elegantUser);

  // Set Authentication Credentials
   ElegantOTA.setAuth(elegantUser.c_str(), elegantPassword.c_str());


  // Note: ElegantOTA listens on the url "/update"
  ElegantOTA.begin(&server);    // Start ElegantOTA

  //ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();
  Serial.println("HTTP server started");

  //Create a new image cache named IMAGE_BW and fill it with white
  UBYTE *BlackImage, *RYImage;
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
  //Paint_NewImage(BlackImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);
  Paint_NewImage(RYImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);

// Serial.println(F("1"));
//   //Select Image
//   Paint_SelectImage(BlackImage);
//   Paint_Clear(WHITE);
//   Paint_SelectImage(RYImage);
//   Paint_Clear(WHITE);

// Serial.println(F("2"));

//#if 0   // show image for array    
  printf("show image for array\r\n");
  EPD_7IN5B_V2_Display(gImage_7in5_V2_b, gImage_7in5_V2_ry);
  DEV_Delay_ms(2000);
//#endif


  /*Horizontal screen*/
  //1.Draw black image
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
  Paint_DrawString_EN(130, 20, "Elijah", &Font24, WHITE, BLACK);
  Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

  //2.Draw red image
  Paint_SelectImage(RYImage);
  Paint_Clear(WHITE);
  Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawString_EN(130, 0, "HOWARD", &Font12, BLACK, WHITE);
  Paint_DrawString_EN(10, 20, "says hello world", &Font12, WHITE, BLACK);
  Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

  printf("EPD_Display\r\n");
  EPD_7IN5B_V2_Display(BlackImage, RYImage);
  DEV_Delay_ms(2000);


  // printf("Clear...\r\n");
  // EPD_7IN5B_V2_Clear();

  // printf("Goto Sleep...\r\n");
  // EPD_7IN5B_V2_Sleep();
  // free(BlackImage);
  // free(RYImage);
  // BlackImage = NULL;
  // RYImage = NULL;

}

/* The main loop -------------------------------------------------------------*/
void loop()
{
    ElegantOTA.loop();
}
