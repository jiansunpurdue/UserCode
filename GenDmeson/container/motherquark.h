#ifndef MOTHERQUARK_H
#define MOTHERQUARK_H

#include "TObject.h"

class motherquark : public TObject {
  public:
     motherquark();
     int pid;
     int charge;
     double pt;
     double eta;
     double phi;
     double mass;
     void clear () {pid = -999; charge = -999; pt = -999; eta = -999; phi = -999; mass = -999;};
  private:
     ClassDef(motherquark , 1)
};

//  ClassImp(motherquark)
#endif
