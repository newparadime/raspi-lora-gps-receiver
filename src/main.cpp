#include <LoRa.h>
#include "GPSLocation.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

int main(void) {
  LoRa.setPins(21, 0, 2);

  if(!LoRa.begin(433E6)) {
    std::cout << "Could not initialize LoRa\n";
  }

  while(true) {
    GPSLocation::NetworkView view;
    uint8_t* viewBuffer = (uint8_t*)&view;

    size_t bytesReady = LoRa.parsePacket(sizeof(GPSLocation::NetworkView));
    size_t bytesReceived;
    if (bytesReady == sizeof(GPSLocation::NetworkView)) {
      for(bytesReceived = 0; bytesReceived < sizeof(GPSLocation::NetworkView) && LoRa.available(); bytesReceived++) {
        viewBuffer[bytesReceived] = (char)LoRa.read();
      }

      if (bytesReceived == sizeof(GPSLocation::NetworkView)) {
        GPSLocation location;
        location.Unpack(view);

        std::cout << "Received valid GPS data packet:\n"
                  << "  latitude     : " << location.latitude << std::endl
                  << "  longitude    : " << location.longitude << std::endl
                  << "  altitude     : " << location.altitude << std::endl
                  << "  numSatellites: " << location.numSatellites << std::endl;
      }
      else {
        std::cout << "ERROR: Failed to read full packet, expected " << sizeof(GPSLocation::NetworkView) << "bytes, received " << bytesReceived << "bytes\n";
      }
    }
    else {
      std::cout << "ERROR: Malformed packet received, expected " << sizeof(GPSLocation::NetworkView) << "bytes, received " << bytesReceived << "bytes\n";
    }
    


    std::this_thread::sleep_for(9s);
  }
    
  return 0;
}