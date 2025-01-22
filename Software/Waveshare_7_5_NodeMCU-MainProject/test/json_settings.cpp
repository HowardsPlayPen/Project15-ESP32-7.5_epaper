#include "json_settings.h"

// These Globals are populated in LoadDefaultParameters() and then can be used to activate WiFi
//
String ssid; // Wifi ssid
String password; // WIFI password

String ota_user;
String ota_password;

bool LoadJsonSettings(JsonDocument& doc)
{
  DeserializationError error = deserializeJson(doc, settings_json);

  if (error) {
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    return false;
  }

  return true;
}

bool LoadDefaultParameters(JsonObject& root )
{
  bool retval = true;
  try
  {
    ssid            = root["ssid"].as<String>();
    password        = root["password"].as<String>();

    //// TODO CHECK IF ota_useer EXISTS BEFORE TRYING TO GET IT
    ota_user        = root["ota_user"].as<String>();
    ota_password    = root["ota_password"].as<String>();
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
    retval = false;
  }
  catch(...)
  {
    Serial.println("ERROR: Caught unspecified exception");
    retval = false;
  }
  
  if(retval && ssid.length() == 0)
  {
    Serial.println("NO SSID configured - update setting.json and enter the right parameters!!");
    retval = false;
  }

  return retval;
}