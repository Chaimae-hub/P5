#include <iostream>
#include <math.h>
#include "sinfm.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

SinFM:: SinFM(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  kv.to_float("I",I);
  kv.to_float("N",N);

  if (!kv.to_float("N",N))
    N = 40; //default value

}

void SinFM::command(long cmd, long note, long vel) {

  if (cmd == 9) {
    bActive = true;
    adsr.start();
    fc=440*pow(2,(note-69.)/12.);
    nota=fc/SamplingRate;
    fm=N*nota;
    I=I*fc;
	A = vel / 127.;

   }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & SinFM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;


  for (unsigned int i=0; i<x.size(); ++i) {

    x[i]=A*sin(N1+I*sin(N2));

    N1+=2*M_PI*nota;
    N2+=2*M_PI*fm;

    while (N1>M_PI)
      N1-=2*M_PI;
    while (N2>M_PI)
      N2-=2*M_PI;
  }

  adsr(x); //apply envelope to x (and updates internal status of adsr)

  return x;
}