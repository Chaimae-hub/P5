#ifndef SINFM
#define SINFM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class SinFM: public upc::Instrument {
    EnvelopeADSR adsr;
    //unsigned int index;
	float A;
	float N1=0,N2=0;
    	float fc,fm,nota,N,I;
    std::vector<float> tbl;
  public:
    SinFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
