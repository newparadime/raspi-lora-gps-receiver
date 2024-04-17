#pragma once
#include <memory>

namespace __private {
  class Base {
    public:
      virtual void MarkStale() { _updated = false; }
      virtual void MarkInvalid() { _valid = false; }
      virtual bool Valid() { return _valid; }
      virtual bool Updated() { return _updated; }

    protected:
      Base() {}
      bool _updated = false;
      bool _valid = false;
  };

  class BaseAggregate {
    public:
      virtual void MarkStale() = 0;
      virtual void MarkInvalid() = 0;
      virtual bool Valid() = 0;
      virtual bool Updated() = 0;

    protected:
      BaseAggregate() {}
  };
}


class GPSStatus : public __private::BaseAggregate {
  public:
    class Float32 : public __private::Base {
      public:
        #pragma pack(push, 1)
        struct NetworkView {
          int32_t digits;
          int16_t exponent;
          int8_t negative;
          uint8_t valid;
          uint8_t updated;
        };
        #pragma pack(pop)
        
        Float32() : n(0) {}
        Float32(const float& n) : n(n) { _valid = true; _updated = true; }
        Float32(const Float32& n) : n(n.n) { _valid = true; _updated = true; }
        Float32(const Float32::NetworkView& view) { Unpack(view); _valid = true; _updated = true; }

        Float32& operator = (const Float32& rhs) { n = rhs.n; _valid = true; _updated = true; return *this; }
        Float32& operator = (const float& rhs) { n = rhs; _valid = true; _updated = true; return *this; }
        operator float() const { return n; }

        Float32::NetworkView Pack();
        Float32& Unpack(Float32::NetworkView view);

      private:
        float n;
    };

    class Int32 : public __private::Base {
      public:
        #pragma pack(push, 1)
        struct NetworkView {
          int32_t n;
          uint8_t valid;
          uint8_t updated;
        };
        #pragma pack(pop)

        Int32() : n(0) {}
        Int32(const int32_t& n) : n(n) { _valid = true; _updated = true; }
        Int32(const Int32& n) : n(n.n) { _valid = true; _updated = true; }
        Int32(const Int32::NetworkView& view) { Unpack(view); _valid = true; _updated = true; }

        Int32& operator = (const Int32& rhs) { n = rhs.n; _valid = true; _updated = true; return *this; }
        Int32& operator = (const int32_t& rhs) { n = rhs; _valid = true; _updated = true; return *this; }
        operator int32_t() const { return n; }

        Int32::NetworkView Pack();
        Int32& Unpack(Int32::NetworkView view);

      private:
        int32_t n;
    };

    class Location : public __private::BaseAggregate {
      public:
        #pragma pack(push, 1)
        struct NetworkView {
          Float32::NetworkView latitude;
          Float32::NetworkView longitude;
        };
        #pragma pack(pop)
        
        Location() : latitude(), longitude() {}
        Location(const Float32& initialLatitude, const Float32& initialLongitude)
          : latitude(initialLatitude), longitude(initialLongitude) { }
        Location(const Location& initialLocation) 
          : latitude(initialLocation.latitude), longitude(initialLocation.longitude) { }
        Location(const Location::NetworkView& view) { Unpack(view); }

        Location& operator = (const Location& rhs) { latitude = rhs.latitude; longitude = rhs.longitude; return *this; }

        Location::NetworkView Pack();
        Location& Unpack(Location::NetworkView view);

        virtual void MarkStale() override;
        virtual void MarkInvalid() override;
        virtual bool Valid() override;
        virtual bool Updated() override;

        Float32 latitude;
        Float32 longitude;
    };

    #pragma pack(push, 1)
    struct NetworkView {
      Location::NetworkView location;
      Float32::NetworkView altitude;
      Int32::NetworkView   numSatellites;
    };
    #pragma pack(pop)

    GPSStatus() : location(), altitude(), numSatellites() {}
    GPSStatus(const Location& initialLocation, const Float32& initialAltitude, const Int32& initialNumSatellites)
      : location(initialLocation), altitude(initialAltitude), numSatellites(initialNumSatellites) { }
    GPSStatus(const Float32& initialLatitude, const Float32& initialLongitude, const Float32& initialAltitude, const Int32& initialNumSatellites)
      : location(initialLatitude, initialLongitude), altitude(initialAltitude), numSatellites(initialNumSatellites) { }
    GPSStatus(const GPSStatus& initialGPSStatus) 
      : location(initialGPSStatus.location), altitude(initialGPSStatus.altitude), numSatellites(initialGPSStatus.numSatellites) { }
    GPSStatus(const GPSStatus::NetworkView& view) { Unpack(view); }

    GPSStatus& operator = (const GPSStatus& rhs) { location = rhs.location; altitude = rhs.altitude; numSatellites = rhs.numSatellites; return *this; }

    GPSStatus::NetworkView Pack();
    GPSStatus& Unpack(GPSStatus::NetworkView view);

    virtual void MarkStale() override;
    virtual void MarkInvalid() override;
    virtual bool Valid() override;
    virtual bool Updated() override;

    Location location = Location();
    Float32 altitude = 0;
    Int32 numSatellites = 0;
};
