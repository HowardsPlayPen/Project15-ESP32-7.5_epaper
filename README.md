# Project15-ESP32-7.5_epaper
Simple ESP32 project including a 7.5 inch Waveshare epaper display. 


Aim is to just have a photo frame like look and display useful information - e.g. weather etc.

![View of project](img/waveshare.jpg)

I did look at installing ESPHome etc on it to show information from my HomeAssistant but decided I wanted to have richer (developer) control - but maybe I will come back to this idea at some point.

Project includes things like 
* OTA updates (far easier than having to update over USB). 
* SSDP to get the IoT to show up in my Network page automatically rather than having to find the IP address
* configuration parameters are stored in a JSON file that is part of the PlatformIO project then compiled into the binary that is uploaded. I found the WifiManager approach more of a headache (for me at least) to configure all the parameters I might want in a project - vs adding them to the JSON.
* Uses PsychicHttp rather than Asyncronousxxx as Psychic seems more performant but also gives good WebSocket support.

I initially started with a project from GitHub to test that the display works.

It is using a bought ESP32 + epaper interface module - which works but it really grated that they used a micro usb and did not have enough de-coupling capacitors to stop it browning out when using the display AND activating the network. I made a small daughter board that connects to the pins to supply power from a USB-C connector (and never had issues since).

Note: I put the software projects in a Software folder because I can then have a Hardware folder if I wish to include a Kicad PCB design alongside it.

Issues right now are that the refresh is really long (i.e. normal for epaper) - but I am sure I saw a project where they managed to do partial refreshes and do it quicker.