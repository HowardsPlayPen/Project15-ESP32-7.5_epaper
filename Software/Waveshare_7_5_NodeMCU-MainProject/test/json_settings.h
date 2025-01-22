#pragma once


// We allow comments to be in the JSON (they are ignored)
#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>


/**
 * NOTE you MUST embed a file in your project (see info on platform.ini below)
 * called "settings.json" which will then contain all the wifi parameters for your network and 
 * whatever other parameters you want to define
 * 
 *  You should have a line like the following in platform.ini to embed the settings.json
 *  "build_flags=-DELEGANTOTA_USE_ASYNC_WEBSERVER=1 -DCOMPONENT_EMBED_TXTFILES=src/settings.json"
 */

// The json file is compiled into the executable and can be accessed by the code
// See https://thingpulse.com/embed-binary-data-on-esp32/
// For background on embedding a file into the executable
extern const uint8_t settings_json[] asm("_binary_src_settings_json_start");

// These will be populated from the JSON file when it is loaded - you can obviously use
// the JSONDocument to pull others out as well in your own code.
extern String ssid; // Wifi ssid
extern String password; // WIFI password

extern String ota_user;
extern String ota_password;

/**
 * Uses the settings.json file that has been embedded with your project to populate the JSONDocument you supply.
 * 
 * Obviously will return false if it has an error.
 * 
 * NOTE it internally uses "settings_json" variable above to access the file.
 * 
 * @param doc 
 * @return true 
 * @return false 
 */
extern bool LoadJsonSettings(JsonDocument& doc);

/**
 * This extracts out the ssid and password parameters
 * (as well as the ota_user and ota_password values if present in the json)
 * 
 * @param root - this object is generally obtained from the "JsonDocument& doc" passed to LoadJsonSettings. 
 * @return true 
 * @return false 
 */
extern bool LoadDefaultParameters(JsonObject& root );