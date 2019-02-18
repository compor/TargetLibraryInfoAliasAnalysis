//
//
//

#include "TargetLibraryInfoAliasAnalysis/TargetLibraryInfoAliasAnalysis.hpp"

#include "TargetLibraryInfoAliasAnalysis/Util.hpp"

#include "llvm/Analysis/AliasAnalysis.h"
// using llvm::FunctionModRefBehavior
// using llvm::createExternalAAWrapperPass
// using llvm::AAResults

#include "llvm/Analysis/TargetLibraryInfo.h"
// using llvm::TargetLibraryInfo

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses

#include "llvm/Support/Debug.h"
// using LLVM_DEBUG macro
// using llvm::dbgs

#define DEBUG_TYPE "tli-aa"

namespace llvm {
class Function;
}

namespace tliaa {

void TLIAAResult::initialize() {
  LLVM_DEBUG(llvm::dbgs() << "initializing TLIAA\n";);
  PureFuncs.reserve(llvm::NumLibFuncs);
  initializeMathFuncs();
}

void TLIAAResult::initializeMathFuncs() {
  using namespace llvm;

  PureFuncs.set(LibFunc_sincospi_stret);
  PureFuncs.set(LibFunc_sincospif_stret);
  PureFuncs.set(LibFunc_acos);
  PureFuncs.set(LibFunc_acos_finite);
  PureFuncs.set(LibFunc_acosf);
  PureFuncs.set(LibFunc_acosf_finite);
  PureFuncs.set(LibFunc_acosh);
  PureFuncs.set(LibFunc_acosh_finite);
  PureFuncs.set(LibFunc_acoshf);
  PureFuncs.set(LibFunc_acoshf_finite);
  PureFuncs.set(LibFunc_acoshl);
  PureFuncs.set(LibFunc_acoshl_finite);
  PureFuncs.set(LibFunc_acosl);
  PureFuncs.set(LibFunc_acosl_finite);
  PureFuncs.set(LibFunc_asin);
  PureFuncs.set(LibFunc_asin_finite);
  PureFuncs.set(LibFunc_asinf);
  PureFuncs.set(LibFunc_asinf_finite);
  PureFuncs.set(LibFunc_asinh);
  PureFuncs.set(LibFunc_asinhf);
  PureFuncs.set(LibFunc_asinhl);
  PureFuncs.set(LibFunc_asinl);
  PureFuncs.set(LibFunc_asinl_finite);
  PureFuncs.set(LibFunc_atan);
  PureFuncs.set(LibFunc_atanf);
  PureFuncs.set(LibFunc_atanh);
  PureFuncs.set(LibFunc_atanh_finite);
  PureFuncs.set(LibFunc_atanhf);
  PureFuncs.set(LibFunc_atanhf_finite);
  PureFuncs.set(LibFunc_atanhl);
  PureFuncs.set(LibFunc_atanhl_finite);
  PureFuncs.set(LibFunc_atanl);
  PureFuncs.set(LibFunc_cbrt);
  PureFuncs.set(LibFunc_cbrtf);
  PureFuncs.set(LibFunc_cbrtl);
  PureFuncs.set(LibFunc_ceil);
  PureFuncs.set(LibFunc_ceilf);
  PureFuncs.set(LibFunc_ceill);
  PureFuncs.set(LibFunc_cos);
  PureFuncs.set(LibFunc_cosf);
  PureFuncs.set(LibFunc_cosh);
  PureFuncs.set(LibFunc_cosh_finite);
  PureFuncs.set(LibFunc_coshf);
  PureFuncs.set(LibFunc_coshf_finite);
  PureFuncs.set(LibFunc_coshl);
  PureFuncs.set(LibFunc_coshl_finite);
  PureFuncs.set(LibFunc_cosl);
  PureFuncs.set(LibFunc_exp10);
  PureFuncs.set(LibFunc_exp10_finite);
  PureFuncs.set(LibFunc_exp10f);
  PureFuncs.set(LibFunc_exp10f_finite);
  PureFuncs.set(LibFunc_exp10l);
  PureFuncs.set(LibFunc_exp10l_finite);
  PureFuncs.set(LibFunc_exp2);
  PureFuncs.set(LibFunc_exp2_finite);
  PureFuncs.set(LibFunc_exp2f);
  PureFuncs.set(LibFunc_exp2f_finite);
  PureFuncs.set(LibFunc_exp2l);
  PureFuncs.set(LibFunc_exp2l_finite);
  PureFuncs.set(LibFunc_exp);
  PureFuncs.set(LibFunc_exp_finite);
  PureFuncs.set(LibFunc_expf);
  PureFuncs.set(LibFunc_expf_finite);
  PureFuncs.set(LibFunc_expl);
  PureFuncs.set(LibFunc_expl_finite);
  PureFuncs.set(LibFunc_expm1);
  PureFuncs.set(LibFunc_expm1f);
  PureFuncs.set(LibFunc_expm1l);
  PureFuncs.set(LibFunc_fabs);
  PureFuncs.set(LibFunc_fabsf);
  PureFuncs.set(LibFunc_fabsl);
  PureFuncs.set(LibFunc_floor);
  PureFuncs.set(LibFunc_floorf);
  PureFuncs.set(LibFunc_floorl);
  PureFuncs.set(LibFunc_log10);
  PureFuncs.set(LibFunc_log10_finite);
  PureFuncs.set(LibFunc_log10f);
  PureFuncs.set(LibFunc_log10f_finite);
  PureFuncs.set(LibFunc_log10l);
  PureFuncs.set(LibFunc_log10l_finite);
  PureFuncs.set(LibFunc_log1p);
  PureFuncs.set(LibFunc_log1pf);
  PureFuncs.set(LibFunc_log1pl);
  PureFuncs.set(LibFunc_log2);
  PureFuncs.set(LibFunc_log2_finite);
  PureFuncs.set(LibFunc_log2f);
  PureFuncs.set(LibFunc_log2f_finite);
  PureFuncs.set(LibFunc_log2l);
  PureFuncs.set(LibFunc_log2l_finite);
  PureFuncs.set(LibFunc_log);
  PureFuncs.set(LibFunc_log_finite);
  PureFuncs.set(LibFunc_logb);
  PureFuncs.set(LibFunc_logbf);
  PureFuncs.set(LibFunc_logbl);
  PureFuncs.set(LibFunc_logf);
  PureFuncs.set(LibFunc_logf_finite);
  PureFuncs.set(LibFunc_logl);
  PureFuncs.set(LibFunc_logl_finite);
  PureFuncs.set(LibFunc_nearbyint);
  PureFuncs.set(LibFunc_nearbyintf);
  PureFuncs.set(LibFunc_nearbyintl);
  PureFuncs.set(LibFunc_rint);
  PureFuncs.set(LibFunc_rintf);
  PureFuncs.set(LibFunc_rintl);
  PureFuncs.set(LibFunc_round);
  PureFuncs.set(LibFunc_roundf);
  PureFuncs.set(LibFunc_roundl);
  PureFuncs.set(LibFunc_sin);
  PureFuncs.set(LibFunc_sinf);
  PureFuncs.set(LibFunc_sinh);
  PureFuncs.set(LibFunc_sinh_finite);
  PureFuncs.set(LibFunc_sinhf);
  PureFuncs.set(LibFunc_sinhf_finite);
  PureFuncs.set(LibFunc_sinhl);
  PureFuncs.set(LibFunc_sinhl_finite);
  PureFuncs.set(LibFunc_sinl);
  PureFuncs.set(LibFunc_sqrt);
  PureFuncs.set(LibFunc_sqrt_finite);
  PureFuncs.set(LibFunc_sqrtf);
  PureFuncs.set(LibFunc_sqrtf_finite);
  PureFuncs.set(LibFunc_sqrtl);
  PureFuncs.set(LibFunc_sqrtl_finite);
  PureFuncs.set(LibFunc_tan);
  PureFuncs.set(LibFunc_tanf);
  PureFuncs.set(LibFunc_tanh);
  PureFuncs.set(LibFunc_tanhf);
  PureFuncs.set(LibFunc_tanhl);
  PureFuncs.set(LibFunc_tanl);
  PureFuncs.set(LibFunc_trunc);
  PureFuncs.set(LibFunc_truncf);
  PureFuncs.set(LibFunc_truncl);
  PureFuncs.set(LibFunc_atan2);
  PureFuncs.set(LibFunc_atan2_finite);
  PureFuncs.set(LibFunc_atan2f);
  PureFuncs.set(LibFunc_atan2f_finite);
  PureFuncs.set(LibFunc_atan2l);
  PureFuncs.set(LibFunc_atan2l_finite);
  PureFuncs.set(LibFunc_fmin);
  PureFuncs.set(LibFunc_fminf);
  PureFuncs.set(LibFunc_fminl);
  PureFuncs.set(LibFunc_fmax);
  PureFuncs.set(LibFunc_fmaxf);
  PureFuncs.set(LibFunc_fmaxl);
  PureFuncs.set(LibFunc_fmod);
  PureFuncs.set(LibFunc_fmodf);
  PureFuncs.set(LibFunc_fmodl);
  PureFuncs.set(LibFunc_copysign);
  PureFuncs.set(LibFunc_copysignf);
  PureFuncs.set(LibFunc_copysignl);
  PureFuncs.set(LibFunc_pow);
  PureFuncs.set(LibFunc_pow_finite);
  PureFuncs.set(LibFunc_powf);
  PureFuncs.set(LibFunc_powf_finite);
  PureFuncs.set(LibFunc_powl);
  PureFuncs.set(LibFunc_powl_finite);
  PureFuncs.set(LibFunc_ldexp);
  PureFuncs.set(LibFunc_ldexpf);
  PureFuncs.set(LibFunc_ldexpl);
  PureFuncs.set(LibFunc_ffs);
  PureFuncs.set(LibFunc_ffsl);
  PureFuncs.set(LibFunc_ffsll);
  PureFuncs.set(LibFunc_fls);
  PureFuncs.set(LibFunc_flsl);
  PureFuncs.set(LibFunc_flsll);
  PureFuncs.set(LibFunc_abs);
  PureFuncs.set(LibFunc_labs);
  PureFuncs.set(LibFunc_llabs);
  PureFuncs.set(LibFunc_sinpi);
  PureFuncs.set(LibFunc_cospi);
  PureFuncs.set(LibFunc_sinpif);
  PureFuncs.set(LibFunc_cospif);
  PureFuncs.set(LibFunc_cabs);
  PureFuncs.set(LibFunc_cabsf);
  PureFuncs.set(LibFunc_cabsl);
}

llvm::AliasResult TLIAAResult::alias(const llvm::MemoryLocation &LocA,
                                     const llvm::MemoryLocation &LocB) {
  return AAResultBase::alias(LocA, LocB);
}

bool TLIAAResult::pointsToConstantMemory(const llvm::MemoryLocation &Loc,
                                         bool OrLocal) {
  return AAResultBase::pointsToConstantMemory(Loc, OrLocal);
}

//

void TLIAAWrapperPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.addRequired<llvm::TargetLibraryInfoWrapperPass>();
  AU.setPreservesAll();
}

} // namespace tliaa

// plugin registration for opt

char tliaa::TLIAAWrapperPass::ID = 0;

static llvm::RegisterPass<tliaa::TLIAAWrapperPass>
    X("tli-aa", PRJ_CMDLINE_DESC("Target Library Info Alias Analysis results"),
      false, false);

static llvm::ImmutablePass *createTLIAAWrapperPass() {
  return new tliaa::TLIAAWrapperPass();
}

static llvm::ImmutablePass *createTLIAAExternalWrapperPass() {
  return llvm::createExternalAAWrapperPass(
      [](llvm::Pass &P, llvm::Function &F, llvm::AAResults &AAR) {
        if (auto *WrapperPass =
                P.getAnalysisIfAvailable<tliaa::TLIAAWrapperPass>()) {
          AAR.addAAResult(WrapperPass->getResult());
        }
      });
}

// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerTLIAAWrapperPass(const llvm::PassManagerBuilder &Builder,
                                     llvm::legacy::PassManagerBase &PM) {
  PM.add(createTLIAAWrapperPass());
  PM.add(createTLIAAExternalWrapperPass());

  return;
}

static llvm::RegisterStandardPasses
    RegisterTLIAAWrapperPass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                             registerTLIAAWrapperPass);

