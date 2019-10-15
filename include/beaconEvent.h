#ifndef _BEACON_EVENT_H 
#define _BEACON_EVENT_H 


#include "TObject.h" 
#include "beaconConsts.h"
#include <stdint.h> 
#include <vector>
#include "beaconCalibrationInfo.h" 

class TGraph; 

#ifdef HAVE_LIBBEACON
struct beacon_event; 
#endif

namespace beacon
{

  class Event : public TObject 
  {

    public: 
      /** Default constructor */
      Event(); 

#ifdef HAVE_LIBBEACON
      /** Constructor from raw data (requires libbeacon.so) */ 
      Event (const beacon_event *event); 
#endif 

      /** Return the raw data for a board and channel. 
       * If there is no data for that channel, return NULL. 
       */ 
      const UChar_t * getRawData(int channel) const { return raw_data[channel].size() ?  &raw_data[channel][0]: 0 ; }

      /** Sets the calibration for this event. This is used to convert from adc to Volts*/ 
      void setCalibrationInfo(const CalibrationInfo & info) { dumpCalibrated(); calibration = info; }  

      int getRunNumber() const { return (int) (event_number / 1000000000l); } 

      /** This creates a calibrated, properly named,  TGraph of this event 
       *
       *  You may choose to provide a TGraph in useme to avoid allocating a new one, but 
       *  deleting it is the responsibility of the user. 
       *
       **/ 
      TGraph * getGraph(int channel = 0, 
                       TGraph * useme = 0) const; 

      /** Retrieves calibrated data. 
       * This returns an internal copy that might get modified if setCalibrationInfo is called and will
       * be lost if this event is deallocated.. 
       **/ 
      const double * getData(int channel = 0) const; 

      /** Returns a copy of calibrated data. Useful if you want to do something with it or w*/ 
      double * copyData(int channel = 0, double * dest = 0 ) const; 

      uint16_t getBufferLength() const { return buffer_length; } 

      uint64_t getEventNumber() const { return event_number; } 

    private: 
      /** The event number */ 
      uint64_t event_number; 
      /** The buffer length */
      uint16_t buffer_length;
      uint8_t board_id; 
      std::vector<UChar_t> raw_data[k::num_chans_per_board]; 
      mutable std::vector<double> data[k::num_chans_per_board];  //!  
      void dumpCalibrated() const;   
      mutable uint64_t calibrated_event_number; //! 
      mutable CalibrationInfo calibration; 

    ClassDef(Event,1); 

  }; 
} 

#endif
