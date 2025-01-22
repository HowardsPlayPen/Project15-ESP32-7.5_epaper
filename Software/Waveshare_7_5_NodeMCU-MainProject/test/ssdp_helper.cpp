
#include "ssdp_helper.hpp"

#include "ESP32SSDP.h"
#include <PsychicHttp.h>

esp_err_t get_schema(PsychicRequest *request)
{
    const char * sch = SSDP.getSchema();
    uint len = strlen(sch);
        
    PsychicResponse response(request);
    response.setCode(200);
    response.setContentType("text/xml");
    response.setContent((const uint8_t*)sch,  (int) len);  

  return response.send();
}


/**
 * Setup the SSDP broadcast - passing in all the parameters to use.
 * 
 * @param params 
 * @return true 
 * @return false 
 */
bool setup_ssdp(const ssdp_helper& params, PsychicHttpServer& server)
{
     //set schema xml url, nees to match http handler
    //"ssdp/schema.xml" if not set
    SSDP.setSchemaURL("schema.xml");
    server.on("/schema.xml", HTTP_GET, get_schema);

    //set port
    //80 if not set
    SSDP.setHTTPPort(80);

    //set device name
    //Null string if not set
    if(params.name.length())
        SSDP.setName(params.name.c_str());

    //set Serial Number
    //Null string if not set
    SSDP.setSerialNumber("001788102201");

    SSDP.setManufacturer("ESP32");
    
    //set device url
    //Null string if not set
    SSDP.setURL(params.url.c_str());
    //set model name
    //Null string if not set
    if(params.modelName.length())
        SSDP.setModelName(params.modelName.c_str());

    //set model description
    //Null string if not set
    SSDP.setModelDescription("This device can be controled by WiFi");
    //set model number
    //Null string if not set
    SSDP.setModelNumber("929000226503");
    //set model url
    //Null string if not set
    if(params.modelUrl.length())
        SSDP.setModelURL(params.modelUrl.c_str());
    //set model manufacturer name
    //Null string if not set    
    //set device type
    //"urn:schemas-upnp-org:device:Basic:1" if not set
    SSDP.setDeviceType("upnp:rootdevice"); //to appear as root device, other examples: MediaRenderer, MediaServer ...rootdevice
    //set server name
    //"Arduino/1.0" if not set
    SSDP.setServerName("SSDPServer/1.0");
    //set UUID, you can use https://www.uuidgenerator.net/
    
    //use SSDP.setUUID("daa26fa3-d2d4-4072-bc7a-a1b88ab4234a", false); for full UUID
    SSDP.setUUID("02e0f4ef-fff3-44f9-9f57-db707a12094d");
    //Set icons list, NB: optional, this is ignored under windows
    SSDP.setIcons(  "<icon>"
                    "<mimetype>image/png</mimetype>"
                    "<height>48</height>"
                    "<width>48</width>"
                    "<depth>24</depth>"
                    "<url>icon48.png</url>"
                    "</icon>");
    //Set service list, NB: optional for simple device
    // SSDP.setServices(  "<service>"
    //                     "<serviceType>urn:schemas-upnp-org:service:SwitchPower:1</serviceType>"
    //                     "<serviceId>urn:upnp-org:serviceId:SwitchPower:1</serviceId>"
    //                     "<SCPDURL>/SwitchPower1.xml</SCPDURL>"
    //                     "<controlURL>/SwitchPower/Control</controlURL>"
    //                     "<eventSubURL>/SwitchPower/Event</eventSubURL>"
    //                     "</service>");

    //Serial.printf("Starting SSDP...\n");
    SSDP.begin();
    return true;
}