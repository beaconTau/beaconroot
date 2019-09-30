#include "beaconStatus.h" 


ClassImp(beacon::Status); 

#ifdef HAVE_LIBBEACON
#include "beacon.h" 

beacon::Status::Status(const beacon_status * s) 
{

  memcpy(global_scalers, s->global_scalers, sizeof(global_scalers)); 
  memcpy(beam_scalers, s->beam_scalers, sizeof(beam_scalers)); 
  memcpy(trigger_thresholds, s->trigger_thresholds, sizeof(trigger_thresholds)); 
  deadtime = s->deadtime; 
  readout_time = s->readout_time; 
  readout_time_ns = s->readout_time_ns; 
  latched_pps_time = s->latched_pps_time; 
  board_id = s->board_id; 
  dynamic_beam_mask = s->dynamic_beam_mask; 
  veto_status = s->veto_status; 
}

#endif

beacon::Status::Status() 
{

  memset(global_scalers,0, sizeof(global_scalers)); 
  memset(beam_scalers, 0, sizeof(beam_scalers)); 
  memset(trigger_thresholds, 0, sizeof(trigger_thresholds)); 
  deadtime = 0; 
  readout_time = 0;
  readout_time_ns = 0;
  latched_pps_time = 0;
  board_id = 0;
  dynamic_beam_mask = 0; 
  veto_status = 0; 
}


