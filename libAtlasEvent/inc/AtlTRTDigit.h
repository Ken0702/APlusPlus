//  
// Author: Oliver Maria Kind <mailto: kind@mail.desy.de>
// Update: $Id$
// Copyright: 2008 (C) Oliver Maria Kind
//
#ifndef ATLAS_AtlTRTDigit
#define ATLAS_AtlTRTDigit
#ifndef ROOT_HepDriftChamberDigit
#include <HepDriftChamberDigit.h>
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif


class AtlTRTDigit : public HepDriftChamberDigit {

protected:
    UInt_t  fDigit;          // TRT digitisation bit pattern
    Int_t   fEndCapOrBarrel; // Hit is barrel or endcap
    Int_t   fPhiSector;      // Phi sector of the hit
    Int_t   fModuleOrWheel;  // Module (barrel) or wheel (endcap) of the hit
    Int_t   fStrawLayer;     // Straw layer of the hit
    Int_t   fStraw;          // Straw number of the hit in a layer
    
public:
    AtlTRTDigit();
    AtlTRTDigit(Int_t Id, Float_t DriftTime, Float_t DriftRadius,
		UInt_t Digit);
    virtual ~AtlTRTDigit();
    virtual void Clear(Option_t* option = "");
    virtual void Print(Option_t* option = "") const;
    static void PrintHeader();
    static void PrintFooter();
          
    inline UInt_t  Digit() const { return fDigit; }
    inline Int_t   EndCapOrBarrel() const { return fEndCapOrBarrel; }
    inline Int_t   PhiSector() const { return fPhiSector; }
    inline Int_t   ModuleOrWheel() const { return fModuleOrWheel; }
    inline Int_t   StrawLayer() const { return fStrawLayer; }
    inline Int_t   Straw() const { return fStraw; }
    
    inline void SetEndCapOrBarrel(Int_t EndCapOrBarrel) {
	fEndCapOrBarrel = EndCapOrBarrel;
    }

    inline void SetPhiSector(Int_t PhiSector) {
	fPhiSector = PhiSector;
    }

    inline void SetModuleOrWheel(Int_t ModuleOrWheel) {
	fModuleOrWheel = ModuleOrWheel;
    }

    inline void SetStrawLayer(Int_t StrawLayer) {
	fStrawLayer = StrawLayer;
    }

    inline void SetStraw(Int_t Straw) {
	fStraw = Straw;
    }

    inline Bool_t  IsHighThrHit() const;
    inline Bool_t  HasSetBitAt(Int_t bit);
    inline Int_t   FirstSetBit();
    inline Int_t   LastSetBit();
    inline Int_t   LeadingEdge();
    inline Int_t   TrailingEdge();
    inline Int_t   GetN_Transitions();
    inline Int_t   GetN_Islands();
    inline Int_t   GetN_Zeros(Int_t island);
    inline Float_t TimeOverThreshold(Option_t* option = "prsnt") const;
    
    ClassDef(AtlTRTDigit,2) // Atlas TRT digitisation
};

//____________________________________________________________________

Bool_t AtlTRTDigit::IsHighThrHit() const {
    //
    // Check if hit is a high-threshold hit
    //
    UInt_t mask = 0x4020100;
    if (fDigit & mask) return kTRUE;
    else return kFALSE;
}

//____________________________________________________________________

Bool_t AtlTRTDigit::HasSetBitAt(Int_t bit) {
    //
    // Check whether a bit is set. Choose the bit in the
    // range: 0 < bit < 28.
    //
    if ( (fDigit >> (27-bit)) & 0x1)
	return kTRUE;
    else return kFALSE;
}

//____________________________________________________________________

Int_t AtlTRTDigit::FirstSetBit() {
  //
  // Returns the first set LT bit in the digit. 1 < bit < 24.
  //
  for (Int_t bit = 1; bit < 28; ++bit) {
    
    // Leave out HT bits
    if ( bit == 1 || bit == 10 || bit == 19 )
      continue;
    
    if ( HasSetBitAt(bit) ) 
      return (bit - (bit-1)/9 - 1);
  }
  return 0;
}

//____________________________________________________________________

Int_t AtlTRTDigit::LastSetBit() {
  //
  // Returns the last set LT bit in the digit. 1 < bit < 24.
  //
  for (Int_t bit = 1; bit < 28; ++bit) {

    Int_t testbit = 28 - bit;
   
    // Leave out HT bits
    if ( testbit == 1 || testbit == 10 || testbit == 19 )
      continue;

    if ( HasSetBitAt(testbit) ) 
      return (testbit - (testbit-1)/9 - 1);
  }  
  return 0;
}  

//____________________________________________________________________

Int_t AtlTRTDigit::LeadingEdge() {
  //
  // Return the leading edge of the signal. For the time being this
  // is just the same as FirstSetBit(), but we might want to change
  // this in the future.
  //
  return FirstSetBit();
}

//____________________________________________________________________

Int_t AtlTRTDigit::TrailingEdge() {
  //
  // Return the trailing edge of the signal. For the time being this
  // is just the same as LastSetBit(), but we might want to change
  // this in the future.
  //
  return LastSetBit();
}

//____________________________________________________________________

Int_t AtlTRTDigit::GetN_Transitions() {
    //
    // Returns the number of transitions in the digit.
    // A transition here means a change in the state
    // of low threshold bits along the bitpattern.
    //
    // The following digit has 5 transitions:
    //
    //          000011111110001111110011
    //
    Bool_t wasSet = kFALSE;
    Bool_t isSet = kFALSE;
    Int_t N_Transitions = 0;

    //////////////////////////////////////////////////////
    // Loop over the digit and see if there is a change
    // in the bit setting from one bit to the next.     //
    //////////////////////////////////////////////////////
    for (Int_t bit = 1; bit < 28; ++bit) {

        // Leave out HT bits
	if ( bit == 1 || bit == 10 || bit == 19 )
	    continue;

	isSet = HasSetBitAt(bit);
	if ( (isSet && !wasSet) || (!isSet && wasSet) )
	    ++N_Transitions;
	wasSet = isSet;
    }
    
    return N_Transitions;
}

//____________________________________________________________________

Int_t AtlTRTDigit::GetN_Islands() {
    //
    // Returns the number of islands in the digit.
    // An island here means a group of subsequenly
    // set bits in the bitpattern.
    //
    // The following digit has 3 islands:
    //
    //          000011111110001111110011
    //
    Int_t N_Islands;
    Int_t Transitions = GetN_Transitions();
    /////////////////////////////////////////////
    // we have to treat even and uneven numbers
    // of transitions differently. First even: //
    /////////////////////////////////////////////
    if ( (Transitions % 2) == 0. ) 
	N_Islands = Transitions/2;
    else 
	// uneven number of transitions
	N_Islands = (Transitions + 1)/2;
    
    return N_Islands;
}

//____________________________________________________________________

Int_t AtlTRTDigit::GetN_Zeros(Int_t hole) {
    //
    // Returns the number of zeros in a given hole.
    //

    // If the chosen hole number is larger than the total 
    // number of holes in the digit the function returns -1.
    if ( hole > (GetN_Islands() - 1) )
	return -1;
    
    else {
	Int_t Count_Holes = 0;
      	Bool_t IsSet = kFALSE;
	Bool_t WasSet = kFALSE;
	Int_t Zeros = 0;
	
	for (Int_t bit = 1; bit < 28 ; ++bit) {

	    // Leave out HT bits
	    if ( bit == 1 || bit == 10 || bit == 19 )
		continue;	        

	    IsSet = HasSetBitAt(bit);

	    // We step in a hole
	    if ( !IsSet && WasSet ) {
		++Count_Holes;
		Zeros = 1;
	    }
	    
	    // We step on an island
	    if ( IsSet && !WasSet && Zeros) {

		// It was the right hole, return result
		if ( Count_Holes == hole )
		    return Zeros;
	    }
		
	    // We are in a hole
	    if ( !IsSet && !WasSet && Count_Holes)
		++Zeros;

	    WasSet = IsSet;
	}
    }

    // This can never happen!
    return -2;
}

//____________________________________________________________________

Float_t AtlTRTDigit::TimeOverThreshold(Option_t* option) const {
    //
    // returns the Time-Over-Threshold of a TRT hit. The option string
    // determines which definition should be used to calculate the
    // Time-Over-Threshold. The following definitions are available:
    //
    //    A) "ctb"    - Definition as to Athena 12.0.6 and Combined Test Beam
    //    B) "smpl"   - Most simple Time-over-Threshold definition
    //    C) "prsnt"  - The present official Time-over-Threshold definition
    //
    //
    //  A) "ctb"
    // ==========
    // This is the standard Time-Over-Threshold of a TRT hit as defined for
    // Combined Test Beam or Athena 12.0.6. The procedure is as follows:
    //
    // 1) We look for a valid trailing edge which in this case is the
    //    very last set bit.
    // 2) We look for a valid leading edge which in this case is the
    //    first set bit.
    // 3) We consider all bits in between the leading and trailing edge set.
    //    The Time-over-Threshold hence is the number of these bits multiplied
    //    by 3.125 ns.
    //
    //
    //  B) "smpl"
    // ==============
    // This is the most simple definition. We count all set bits and multiply
    // the result by 3.125 ns. We do not care about leading edges in the very
    // first or trailing edges in the very last bit.
    //
    //
    //  C) "prsnt"
    // ==============
    // This definition is similar to definition A) but the trailing edge
    // is not allowed to be situated in the very last (the 24th) time bin.
    // This is the present official Time-over-Threshold of the TRT hits.
    //
    //
    
    TString opt = option;
    opt.ToLower();

    Float_t binWidth = 3.125;
    Float_t TimeOverThreshold = 0.;
    
    if ( opt == "ctb" ) {

	//////////////////////////////////////////////////////////////
	// This piece of code is copied from the ATHENA code in:
	//
	// InnerDetector/InDetRawEvent/
	// InDetRawData/src/TRT_TB04_RawData.cxx                   
	//
	// In Addition the code is corrected for a bug returning
	// negative Time-over-Threshold for bit patterns with zeros
	// only.                                                    //
        //////////////////////////////////////////////////////////////
	//
        // UInt_t mask = 0x02000000;
	//
	// Int_t LE;
        // for (LE = 0; LE < 24; ++LE) {
        //	if (fDigit & mask)
        //	    break;
        //	    
        //	 mask >>= 1;
        //	 if (LE == 7 || LE == 15)
        //	    mask >>= 1;
        // }
	//
 	// mask = 0x00000001;
	//
	// Int_t TE;
	// for (TE = 0; TE < 24; ++TE) {
	//     if (fDigit & mask)
	// 	break;
	//    
	//    mask <<= 1;
	//    if (TE == 7 || TE == 15)
	//	mask <<= 1;
	// }
	//
	// // We correct for the bug
	// //
	// if ( (LE == 24) && (TE == 24) )
	//       TimeOverThreshold = 0.;
	// else
	//       TimeOverThreshold = (24 - LE - TE) * binWidth;
	//
	//                                                          // 
	//////////////////////////////////////////////////////////////

	///////////////////////////////////////////
	// Time-over-Threshold Definition A
	// ================================      //
	///////////////////////////////////////////
	
	UInt_t mask = 0x0000001;

	Int_t TE;
    	for (TE = 0 ; TE < 24; ++TE) {
	    if ( mask & fDigit )
		break;
	    mask <<= 1;
	
	    // Leave out HT bits
	    if ( TE == 7 || TE == 15 )
		mask <<= 1;
	}
	
	// If we have only zeros in the digit we can as well stop
	// here and return zero Time-over-Threshold.
	//
	if ( TE == 24 ) return 0.;

	// Step 2: leading edge
	//
	Int_t LE;
	mask = 0x2000000;
    
	for (LE = 0; LE < 24; ++LE) {
	    if ( mask & fDigit )
		break;
	    mask >>= 1;
	
	    // Leave out HT bits
	    if ( LE == 7 || LE == 15 )
		mask >>= 1;
	}

	TimeOverThreshold = (24 - LE - TE) * binWidth;

    } else if ( opt == "smpl" ) {

	//////////////////////////////////////////
	// Time-over-Threshold Definition B
	// ================================     //
	//////////////////////////////////////////
	
	// We just count the set bits
	//
	Int_t set_bits = 0;
	for(Int_t bit=1; bit<28;++bit){
	    if ( bit == 1 || bit == 10 || bit == 19 ) continue;
	    if ( (fDigit >> (27-bit)) & 0x1) ++set_bits;
	}
	
	TimeOverThreshold = set_bits * binWidth;

    } else if ( opt == "prsnt" ) {

	///////////////////////////////////////////////////////////
	// Time-over-Threshold Definition C
	// ================================
	// This piece of code is copied from the ATHENA code in:
	// InnerDetector/InDetRecEvent/InDetPrepRawData/
	// InDetPrepRawData/TRT_DriftCircle.h                    //
	///////////////////////////////////////////////////////////
	UInt_t mask = 0x02000000;

	Int_t LE;
	for (LE = 0; LE < 24; ++LE) {
	    
	    if( fDigit & mask ) break;
	    mask >>= 1;
	    if ( LE == 7 || LE == 15 ) mask >>= 1;
	}
	
	mask = 0x00000001;
	Bool_t SawZero = kFALSE;

	Int_t TE;
	for (TE = 0; TE < 24; ++TE) {
	    
	    if ( (fDigit & mask) && SawZero )
		break;
	    else if ( !(fDigit & mask) )
		SawZero = kTRUE;
	    
	    mask <<= 1;
	    if (TE == 7 || TE == 15)
		mask <<= 1;
	}

	if ( (24 == LE) || (24 == TE) || (0 == TE) || (23 == LE) ) 
	    return 0.;
	
	TimeOverThreshold = (24 - LE - TE) * binWidth;
    }

    return TimeOverThreshold;
}


#endif

