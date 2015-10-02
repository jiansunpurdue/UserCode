#ifndef __HFCAND_V2_H
#define __HFCAND_V2_H

#include "hfcand.hh"

#include "snglhfcand_v2.hh"

class TClonesArray;

class hfcand_v2 : public hfcand
{
    private:
        TClonesArray *GetCand() const {return HFcand;}
        unsigned int nhfcand;
        TClonesArray *HFcand;

        long int runNum;
        long int evtNum;

		float BSx;
		float BSy;
		float BSz;

    public:
        hfcand_v2();
        ~hfcand_v2();

        void Reset();

        void set_nhfcand  (const unsigned int in) {nhfcand = in;}
        int  get_nhfcand () const {return nhfcand;}

        void set_runNum(const long int in) {runNum = in;}
        void set_evtNum(const long int in) {evtNum = in;}
		void set_BSx(const float in) {BSx = in;}
		void set_BSy(const float in) {BSy = in;}
		void set_BSz(const float in) {BSz = in;}

        long int get_runNum() {return runNum;}
        long int get_evtNum() {return evtNum;}
		float get_BSx() {return BSx;}
		float get_BSy() {return BSy;}
		float get_BSz() {return BSz;}


        // Routines to manipulate the particle array...
        void AddCand          (const unsigned int itrk);
        snglhfcand* get_hfcand(const unsigned int itrk) const;

        ClassDef(hfcand_v2,1)
};

#endif 






