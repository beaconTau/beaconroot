#include "beaconHk.h" 

ClassImp(beacon::Hk); 

#ifdef HAVE_LIBBEACON

#include "beacon.h" 

beacon::Hk::Hk(const beacon_hk * hk) 
{
    unixTime=hk->unixTime; 
    unixTimeMilliSecs=hk->unixTimeMillisecs; 
    temp_board=hk->temp_board; 
    temp_adc=hk->temp_adc; 
    frontend_current=hk->frontend_current; 
    aux_current=hk->aux_current; 
    ant_current=hk->ant_current; 
    gpio_state= (gpio_power_state) hk->gpio_state; 
    disk_space_kB=hk->disk_space_kB; 
    free_mem_kB=hk->free_mem_kB;  
}
#endif


beacon::Hk::Hk() 
{
    unixTime=0; 
    unixTimeMilliSecs=0; 
    temp_board=0; 
    temp_adc=0; 
    frontend_current=0; //mA
    adc_current=0; 
    aux_current=0; 
    gpio_state=GPIO_POWER_NONE; 
    disk_space_kB=0; 
    free_mem_kB=0;  
}

