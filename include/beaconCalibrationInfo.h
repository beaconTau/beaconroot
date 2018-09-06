#ifndef BEACON_CALIBRATION_INFO_HH
#define BEACON_CALIBRATION_INFO_HH

#include "TObject.h" 
#include "beaconConsts.h" 

namespace beacon
{

  /** The CalibrationInfo knows the conversions between
   *  raw and physical units 
   *  */ 
  class CalibrationInfo   : public TObject
  {

    public: 
      /* Construct the default CalibrationInfo */ 
      CalibrationInfo(); 

      double getVoltageCalibration( int chan) const { return voltage_calibration[chan]; } 
      double getTimeCalibration() const { return time_calibration; } 
      double getCableDelay(int chan) const { return 0; } 

      units getTimeUnits()const  { return t; }
      units getVoltageUnits()const  { return v; }

    private: 
      double time_calibration; 
      double voltage_calibration[k::num_chans_per_board]; 
      units t; 
      units v; 
      
      ClassDef(CalibrationInfo,1); 
  }; 

}

#endif
