#include "beaconCalibrationInfo.h" 

ClassImp(beacon::CalibrationInfo); 

beacon::CalibrationInfo::CalibrationInfo() 
{
  time_calibration = 2; 
  t = UNITS_NS; 

  for (int chan = 0; chan < k::num_chans_per_board; chan++)
  {
    voltage_calibration[chan] = 1; 
  }

  v = UNITS_ADC; 
}
