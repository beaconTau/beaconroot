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

    ClassDef(Hk,1); 
  }; 
}

#endif


