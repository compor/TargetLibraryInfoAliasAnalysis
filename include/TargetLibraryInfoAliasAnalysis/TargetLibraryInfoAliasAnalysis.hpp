//
//
//

#pragma once

#include "TargetLibraryInfoAliasAnalysis/Config.hpp"

#include "llvm/Analysis/AliasAnalysis.h"
// using llvm::AAResultBase

#include "llvm/Pass.h"
// using llvm::ImmutablePass

#include "llvm/ADT/BitVector.h"
// using llvm::BitVector

#include <memory>
// using std::unique_ptr

namespace llvm {
class MemoryLocation;
class Module;
} // namespace llvm

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

// Legacy wrapper pass to provide the TLIAAResult object.
class TLIAAWrapperPass : public llvm::ImmutablePass {
  std::unique_ptr<TLIAAResult> Result;

public:
  static char ID;

  TLIAAWrapperPass() : llvm::ImmutablePass(ID) {}

  TLIAAResult &getResult() { return *Result; }
  const TLIAAResult &getResult() const { return *Result; }

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool doInitialization(llvm::Module &M) override {
    Result.reset(new TLIAAResult(
        getAnalysis<llvm::TargetLibraryInfoWrapperPass>().getTLI()));
    return false;
  }

  bool doFinalization(llvm::Module &M) override {
    Result.reset();
    return false;
  }
};

} // namespace tliaa

// TODO not sure if the new passmanager passes are required to be in the llvm
// namespace if defined out of source tree
namespace llvm {

// Analysis pass providing a never-invalidated alias analysis result.
class TLIAA : public AnalysisInfoMixin<TLIAA> {
  friend AnalysisInfoMixin<TLIAA>;

  static char PassID;

public:
  using Result = tliaa::TLIAAResult;

  tliaa::TLIAAResult run(Function &F, AnalysisManager<Function> &AM) {
    return tliaa::TLIAAResult(AM.getResult<TargetLibraryAnalysis>(F));
  }
};

} // namespace llvm
