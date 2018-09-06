#ifndef _BEACON_CONSTS_H
#define _BEACON_CONSTS_H

namespace beacon
{
  namespace k
  {
    const int num_chans_per_board = 8; 
    const int num_beams = 8; 
    const int num_scalers = 3; 
  }

  enum trig_pol
  {
    TRIGGER_HPOL = 0, 
    TRIGGER_VPOL = 1
  }; 

  enum trig_type 
  {
    TRIGGER_NONE = 0, 
    TRIGGER_SW = 1, 
    TRIGGER_RF = 2, 
    TRIGGER_EXT = 4 
  } ;

  enum sync_problem
  {
    SYNC_PROBLEM_NONE = 0, 
    SYNC_PROBLEM_SW_HW_BUFFER_MISMATCH=1, 
  } ;

  enum scaler_type
  {
    SCALER_SLOW, 
    SCALER_SLOW_GATED,
    SCALER_FAST
  } ;


  enum gpio_power_state
  {
    GPIO_POWER_NONE =0, 
    GPIO_FPGA_POWER= 1, 
    GPIO_SPI_ENABLE = 2,

  }; 

  enum units
  {
    UNITS_NS, 
    UNITS_SAMPLE, 
    UNITS_ADC, 
    UNITS_MV 
  }; 

  const char * getUnitString(units u); 

}

#endif
