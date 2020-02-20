#include "beaconCalibrationInfo.h" 

ClassImp(beacon::CalibrationInfo); 

beacon::CalibrationInfo::CalibrationInfo(int run) 
  : run(run) 
{
  time_calibration = 2; 
  t = UNITS_NS; 

  for (int chan = 0; chan < k::num_chans_per_board; chan++)
  {
    if (run > 1500 && run < 2000 && chan == 7) 
    {
      voltage_calibration[chan] = -1; 
    }
    else
    {
      voltage_calibration[chan] = 1; 
    }
  }

  v = UNITS_ADC; 
}
