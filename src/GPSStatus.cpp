#include <cmath>

#include "GPSStatus.h"

#include "EndianTranslation.h"

GPSStatus::Float32::NetworkView GPSStatus::Float32::Pack() {
  GPSStatus::Float32::NetworkView view = { 0, 0, 0 };
  view.negative = n < 0 ? -1 : 1;
  float nLocal = n * view.negative;  
  
  static constexpr float UINT32_MAX_LOG10 = log10(UINT32_MAX);
  view.exponent = ceil(log10(nLocal) - UINT32_MAX_LOG10);

  nLocal *= pow(10.0, -view.exponent);
  
  view.digits = htonl(static_cast<uint32_t>(nLocal));
  view.exponent = htons(view.exponent);

  view.updated = _updated;
  view.valid = _valid;

  return view;
}

GPSStatus::Float32& GPSStatus::Float32::Unpack(GPSStatus::Float32::NetworkView view) {
  n = ntohl(view.digits);
  n *= view.negative;
  
  view.exponent = ntohs(view.exponent);

  n *= pow(10.0, view.exponent);
  
  _valid = view.valid;
  _updated = view.updated;

  return *this;
}


GPSStatus::Int32::NetworkView GPSStatus::Int32::Pack() {
  GPSStatus::Int32::NetworkView view;
  
  view.n = htonl(n);
  
  view.updated = _updated;
  view.valid = _valid;

  return view;
}

GPSStatus::Int32& GPSStatus::Int32::Unpack(GPSStatus::Int32::NetworkView view) {
  n = ntohl(view.n);

  _valid = true;
  _updated = true;

  return *this;
}


GPSStatus::Location::NetworkView GPSStatus::Location::Pack() {
  GPSStatus::Location::NetworkView view;

  view.latitude = latitude.Pack();
  view.longitude = longitude.Pack();

  return view;
}

GPSStatus::Location& GPSStatus::Location::Unpack(GPSStatus::Location::NetworkView view) {
  latitude.Unpack(view.latitude);
  longitude.Unpack(view.longitude);

  return *this;
}

void GPSStatus::Location::MarkStale() {
  latitude.MarkStale();
  latitude.MarkStale();
}

void GPSStatus::Location::MarkInvalid() {
  latitude.MarkInvalid();
  longitude.MarkInvalid();
}

bool GPSStatus::Location::Valid() {
  return latitude.Valid() && longitude.Valid();
}

bool GPSStatus::Location::Updated() {
  return latitude.Updated() || longitude.Updated();
}


GPSStatus::NetworkView GPSStatus::Pack() {
  GPSStatus::NetworkView view;
  
  view.location = location.Pack();
  view.altitude = altitude.Pack();
  view.numSatellites = numSatellites.Pack();

  return view;
}

GPSStatus& GPSStatus::Unpack(GPSStatus::NetworkView view) {
  location.Unpack(view.location);
  altitude.Unpack(view.altitude);
  numSatellites.Unpack(view.numSatellites);

  return *this;
}

void GPSStatus::MarkStale() {
  location.MarkStale();
  altitude.MarkStale();
  numSatellites.MarkStale();
}

void GPSStatus::MarkInvalid() {
  location.MarkInvalid();
  altitude.MarkInvalid();
  numSatellites.MarkInvalid();
}

bool GPSStatus::Valid() {
  return location.Valid() && altitude.Valid() && numSatellites.Valid();
}

bool GPSStatus::Updated() {
  return location.Updated() || altitude.Updated() || numSatellites.Updated();
}

