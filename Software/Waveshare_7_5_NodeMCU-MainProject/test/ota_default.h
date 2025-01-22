#pragma once

#include <stdlib.h>

#include <PsychicHttp.h>

extern unsigned long ota_progress_millis;

extern void onOTAStart();

extern void onOTAProgress(size_t current, size_t final);

extern void onOTAEnd(bool success);

// Allow any server to connect (i.e. turn off CORS)
// A web browser will send a pre-fetch of Http type "HTTP_OPTIONS" to check CORS status
extern void disable_cors(PsychicHttpServer& server);