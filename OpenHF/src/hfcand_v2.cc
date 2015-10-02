#include "TClonesArray.h"
#include "../interface/hfcand_v2.hh"

ClassImp(hfcand_v2)

using namespace std;

static const unsigned int NCAND = 0;

    //_________________
hfcand_v2::hfcand_v2()
{
    HFcand  = new TClonesArray("snglhfcand_v2",NCAND);
    Reset();
}

//_________________
hfcand_v2::~hfcand_v2()
{
    HFcand->Clear("c");
    return;
}

//_________________
snglhfcand* hfcand_v2::get_hfcand (const unsigned int itrk) const 
{
    snglhfcand_v2 *Particle = (snglhfcand_v2 *) GetCand()->UncheckedAt(itrk);
    return Particle;
}

//_________________
void hfcand_v2::Reset()
{
    HFcand->Clear("c");
    if (nhfcand>NCAND)
    {
        HFcand->Expand(NCAND); //.. reset to the orignal value
    }
    nhfcand = 0;
    runNum = -1;
    evtNum = -1;

	BSx = -999.;
	BSy = -999.;
	BSz = -999.;

    return;
}

//_____________________________________
void  hfcand_v2::AddCand(const unsigned int itrk)
{
    TClonesArray &Particle = *HFcand;
    new(Particle[itrk]) snglhfcand_v2();
    return;
}
