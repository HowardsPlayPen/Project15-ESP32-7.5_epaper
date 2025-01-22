
#pragma once

#include <Arduino.h>

typedef struct 
{
    // If any of these items are left empty then the default is used.
    String url;
    String name;
    String manufacturer;
    String modelName;
    String modelUrl;


} ssdp_helper;

class PsychicHttpServer;

/**
 * Setup the SSDP broadcast - passing in all the parameters to use.
 * 
 * @param params 
 * @return true 
 * @return false 
 */
bool setup_ssdp(const ssdp_helper& params, PsychicHttpServer& server);