#include "beaconEvent.h" 

ClassImp(beacon::Event);
#include "TGraph.h" 
#include "TAxis.h" 
#include "TBuffer.h" 
#include "TClass.h" 


#ifdef HAVE_LIBBEACON
#include "beacon.h" 

beacon::Event::Event(const beacon_event * event) 
{
  event_number = event->event_number; 
  buffer_length = event->buffer_length; 
  board_id = event->board_id[0]; 



  for (int c = 0; c < k::num_chans_per_board; c++)
  {
    raw_data[c].resize(buffer_length); 
    for (int i = 0; i < buffer_length; i++)
    {
      raw_data[c][i] = event->data[0][c][i]; 
    }
  }
}
#endif

beacon::Event::Event() 
{
  event_number = 0;
  buffer_length = 0; 
  calibrated_event_number = 0; 
  board_id = 0; 
}

void beacon::Event::dumpCalibrated() const
{
  for (int c = 0; c < k::num_chans_per_board; c++)
  {
    data[c].clear(); 
  }
}

const double * beacon::Event::getData(int c) const 
{

  if (event_number!= calibrated_event_number) dumpCalibrated(); 

  if (calibration.getRunNumber() != getRunNumber()) 
  {
    calibration = CalibrationInfo(getRunNumber()); 
  }

  //This is the calibration part 
  if (!data[c].size() && raw_data[c].size())
  {
    calibrated_event_number = event_number; 
    data[c].resize(buffer_length); 
    for (int i = 0; i < buffer_length; i++) 
    {
      data[c][i] = (raw_data[c][i] - calibration.getBaseline()) * calibration.getVoltageCalibration(c); 
    }
  }

  return data[c].size() ? &data[c][0] : 0; 
}


double * beacon::Event::copyData(int c, double * destination) const
{
  if (!destination) destination = new double[buffer_length]; 

  const double * the_data = getData(c); 
  if (!the_data)
  {
    memset(destination,0,buffer_length*sizeof(double)); 
  }
  else
  {
    memcpy(destination, the_data, buffer_length * sizeof(double)); 
  }

  return destination; 
}

TGraph * beacon::Event::getGraph(int channel, TGraph * g) const
{
  if (!g) g = new TGraph(buffer_length); 
  else g->Set(buffer_length); 

  copyData(channel, g->GetY()); 


  for (int i = 0; i < g->GetN(); i++) 
  {
    double offset = calibration.getCableDelay(channel); 
    g->GetX()[i] = i * calibration.getTimeCalibration() - offset;
  }

  g->GetYaxis()->SetTitle(getUnitString(calibration.getVoltageUnits())); 
  g->GetXaxis()->SetTitle(getUnitString(calibration.getTimeUnits())); 

  TString title;
  title.Form("Channel %d\n",channel); 
  g->SetTitle(title.Data()); 
  return g; 

}








