//
//
//

#pragma once

#include "TargetLibraryInfoAliasAnalysis/Config.hpp"

#include "llvm/Analysis/AliasAnalysis.h"
// using llvm::AAResultBase

#include "llvm/ADT/BitVector.h"
// using llvm::BitVector

namespace llvm {
class MemoryLocation;
}

namespace tliaa {

class TLIAAResult : public llvm::AAResultBase<TLIAAResult> {
  friend llvm::AAResultBase<TLIAAResult>;

  const llvm::TargetLibraryInfo &TLI;
  llvm::BitVector PureFuncs;

  void initializeMathFuncs();
  void initialize();

public:
  explicit TLIAAResult(const llvm::TargetLibraryInfo &TLI)
      : AAResultBase(), TLI(TLI) {
    initialize();
  }

  TLIAAResult(TLIAAResult &&Other) : TLIAAResult(Other.TLI) {}

  llvm::AliasResult alias(const llvm::MemoryLocation &LocA,
                          const llvm::MemoryLocation &LocB);

  bool pointsToConstantMemory(const llvm::MemoryLocation &Loc, bool OrLocal);
};

} // namespace tliaa
