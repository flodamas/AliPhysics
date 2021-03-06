/************************************************************************************
 * Copyright (C) 2017, Copyright Holders of the ALICE Collaboration                 *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *     * Redistributions of source code must retain the above copyright             *
 *       notice, this list of conditions and the following disclaimer.              *
 *     * Redistributions in binary form must reproduce the above copyright          *
 *       notice, this list of conditions and the following disclaimer in the        *
 *       documentation and/or other materials provided with the distribution.       *
 *     * Neither the name of the <organization> nor the                             *
 *       names of its contributors may be used to endorse or promote products       *
 *       derived from this software without specific prior written permission.      *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL ALICE COLLABORATION BE LIABLE FOR ANY              *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES       *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS    *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     *
 ************************************************************************************/
#ifndef ALIEMCALTRACKSELRESULTCOMBINED_H
#define ALIEMCALTRACKSELRESULTCOMBINED_H

#include <TObject.h>
#include <TClonesArray.h>
#include <exception>
#include <sstream>
#include <string>
#include "AliEmcalTrackSelResultPtr.h"

class TObjArray;

namespace PWG {

namespace EMCAL {

class AliEmcalTrackSelResultCombined : public TObject {
public:
  class IndexException : public std::exception {
  public:
    IndexException(int index) : std::exception(), fIndex(index), fText() {
      std::stringstream msgbuilder;
      msgbuilder << "Index " << index << " out of range.";
      fText = msgbuilder.str();
    }
    virtual ~IndexException() throw () {}

    const char *what() const throw() { return fText.data(); }
    int GetIndex() const { return fIndex; }

  private:
    int fIndex;
    std::string fText;
  };
  
  AliEmcalTrackSelResultCombined();
  AliEmcalTrackSelResultCombined(const TObjArray *singleSelPointers);
  virtual ~AliEmcalTrackSelResultCombined() {}

  AliEmcalTrackSelResultPtr &operator[](int index) const;
  Int_t GetNumberOfSelectionResults() const;

private:
  TClonesArray                   fData;    ///< Single 

  /// \cond CLASSIMP
  ClassDef(AliEmcalTrackSelResultCombined, 1);
  /// \endcond
};

}

}
#endif