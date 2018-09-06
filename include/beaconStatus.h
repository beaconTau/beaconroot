#ifndef _BEACON_STATUS_H 
#define _BEACON_STATUS_H 

#include "TObject.h" 
#include "beaconConsts.h" 

#ifdef HAVE_LIBBEACON
struct beacon_status; 
#endif


namespace beacon
{
  class Status : public TObject
  {
    public:
      /** Default constructor */ 
      Status(); 

#ifdef HAVE_LIBBEACON
        /** Constructor from raw data (requires libbeacon.so) */ 
      Status (const beacon_status *status); 
#endif 

      uint16_t global_scalers[k::num_scalers];  //!< The overall scalers 
      uint16_t beam_scalers[k::num_scalers][k::num_beams]; //!< The scalers for each beam 
      uint32_t deadtime; //!< Deadtime fraction (units tbd) 
      uint32_t readout_time; //!< CPU time of readout
      uint32_t readout_time_ns; //!< CPU time ns of readout
      uint32_t trigger_thresholds[k::num_beams]; //!< Trigger thresholds 
      uint64_t latched_pps_time; //!< timestamp corresponding to a pps time
      uint8_t  board_id; //!< the board number assigned at startup 
      uint32_t dynamic_beam_mask; 

      ClassDef(Status,1); 
  };
}








#endif
