#ifndef BEACON_HK_H
#define BEACON_HK_H


#include "beaconConsts.h" 
#include "TObject.h" 

#ifdef HAVE_LIBBEACON
struct beacon_hk; 
#endif


namespace beacon
{
  class Hk  : public TObject 
  {

    public: 

    Hk(); 
#ifdef HAVE_LIBBEACON
    Hk (const beacon_hk * hk); 
#endif
    uint32_t unixTime; 
    uint16_t unixTimeMilliSecs; 
    int8_t temp_board; 
    int8_t temp_adc; 
    uint16_t frontend_current; //mA
    uint16_t adc_current; 
    uint16_t aux_current; 
    uint16_t ant_current; 

    gpio_power_state gpio_state; 

    uint32_t disk_space_kB; 
    uint32_t free_mem_kB;  

    //just to confuse people, I'll use double32_t's here 
    Double32_t inv_batt_V; //[0,102.4,10]
    Double32_t cc_batt_V; //[0,102.4,10]
    Double32_t pv_V; //[0,819.2,13]
    uint8_t cc_daily_Ah;  
    Double32_t cc_daily_kWh; //[0,25.6,8]; 

    ClassDef(Hk,2); 
  }; 
}

#endif


