#include <LoRa.h>
#include "GPSStatus.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

int main(void) {
  LoRa.setPins(3, 0, 2);

  if(!LoRa.begin(433E6)) {
    std::cout << "Could not initialize LoRa\n";
  }

  while(true) {
    GPSStatus::NetworkView view;
    uint8_t* viewBuffer = (uint8_t*)&view;

    size_t bytesReady = LoRa.parsePacket(sizeof(GPSStatus::NetworkView));
    size_t bytesReceived;
    if (bytesReady == sizeof(GPSStatus::NetworkView)) {
      for(bytesReceived = 0; bytesReceived < sizeof(GPSStatus::NetworkView) && LoRa.available(); bytesReceived++) {
        viewBuffer[bytesReceived] = (char)LoRa.read();
      }

      if (bytesReceived == sizeof(GPSStatus::NetworkView)) {
        GPSStatus status;
        status.Unpack(view);

        std::cout << "Received valid GPS data packet:\n"
                  << "  latitude     : " << status.location.latitude << std::endl
                  << "  longitude    : " << status.location.longitude << std::endl
                  << "  altitude     : " << status.altitude << std::endl
                  << "  numSatellites: " << status.numSatellites << std::endl;
      }
      else {
        std::cout << "ERROR: Failed to read full packet, expected " << sizeof(GPSStatus::NetworkView) << "bytes, received " << bytesReceived << "bytes\n";
      }
    }
    else {
      std::cout << "ERROR: Malformed packet received, expected " << sizeof(GPSStatus::NetworkView) << "bytes, received " << bytesReceived << "bytes\n";
    }
    


    std::this_thread::sleep_for(9s);
  }
    
  return 0;
}