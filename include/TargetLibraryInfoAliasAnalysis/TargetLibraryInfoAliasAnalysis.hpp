//
//
//

#pragma once

#include "TargetLibraryInfoAliasAnalysis/Config.hpp"

#include "llvm/Analysis/AliasAnalysis.h"
// using llvm::AAResultBase

#include "llvm/Pass.h"
// using llvm::ImmutablePass
// using llvm::PreservedAnalyses

#include "llvm/IR/PassManager.h"
// using llvm::FunctionAnalysisManager

#include "llvm/ADT/BitVector.h"
// using llvm::BitVector

#include <memory>
// using std::unique_ptr

namespace llvm {
class MemoryLocation;
class Module;
class Function;
class ImmutableCallSite;
} // namespace llvm

namespace tliaa {

class TLIAAResult : public llvm::AAResultBase<TLIAAResult> {
  friend AAResultBase<TLIAAResult>;

  const llvm::TargetLibraryInfo &TLI;
  llvm::BitVector PureFuncs;

  void initializeMathFuncs();
  void initialize();

  bool isPureFunc(const llvm::Function &Func) const;

public:
  explicit TLIAAResult(const llvm::TargetLibraryInfo &TLI)
      : AAResultBase(), TLI(TLI) {
    initialize();
  }

  TLIAAResult(TLIAAResult &&Other)
      : AAResultBase(std::move(Other)), TLI(Other.TLI),
        PureFuncs(Other.PureFuncs) {}

  bool invalidate(llvm::Function &, const llvm::PreservedAnalyses &,
                  llvm::FunctionAnalysisManager::Invalidator &) {
    return false;
  }

  llvm::FunctionModRefBehavior getModRefBehavior(llvm::ImmutableCallSite CS);

  llvm::FunctionModRefBehavior getModRefBehavior(const llvm::Function *Func);
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

// Analysis pass providing a never-invalidated alias analysis result.
class TLIAA : public llvm::AnalysisInfoMixin<TLIAA> {
  friend AnalysisInfoMixin<TLIAA>;

  static llvm::AnalysisKey Key;

public:
  using Result = TLIAAResult;

  Result run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    return TLIAAResult{FAM.getResult<llvm::TargetLibraryAnalysis>(F)};
  }
};

} // namespace tliaa
