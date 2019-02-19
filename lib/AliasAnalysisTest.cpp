//===--- AliasAnalysisTest.cpp - Mixed TBAA unit tests --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "TargetLibraryInfoAliasAnalysis/TargetLibraryInfoAliasAnalysis.hpp"

using namespace llvm;

namespace {

struct AATestPass : FunctionPass {
  static char ID;
  AATestPass() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<AAResultsWrapperPass>();
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    AliasAnalysis &AA = getAnalysis<AAResultsWrapperPass>().getAAResults();
    //auto &TAA = getAnalysis<tliaa::TLIAAWrapperPass>().getResult();

    SetVector<Value *> Pointers;
    for (Argument &A : F.args())
      //if (A.getType()->isPointerTy())
        Pointers.insert(&A);
    //for (Instruction &I : instructions(F))
      //if (I.getType()->isPointerTy())
        //Pointers.insert(&I);

    for (Instruction &I : instructions(F))
      if (auto *call = dyn_cast<CallInst>(&I)) {
        Pointers.insert(&I);
        //(void)TAA.getModRefBehavior(call->getCalledFunction());
        (void)AA.getModRefBehavior(call->getCalledFunction());
      }

    for (Value *P1 : Pointers)
      for (Value *P2 : Pointers) {
        //(void)AA.alias(P1, MemoryLocation::UnknownSize, P2,
                       //MemoryLocation::UnknownSize);
        //(void)TAA.alias(P1, P2);
        //(void)TAA.getModRefBehavior(P1);
        //(void)TAA.getModRefBehavior(P2);
      }

    return false;
  }
};
} // namespace

char AATestPass::ID = 0;

static llvm::RegisterPass<AATestPass> X("aa-test", "Test Alias Analysis client",
                                        false, true);

static void registerAATestPass(const PassManagerBuilder &Builder,
                               legacy::PassManagerBase &PM) {
  PM.add(new AATestPass());

  return;
}

static RegisterStandardPasses
    RegisterAATestPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerAATestPass);
