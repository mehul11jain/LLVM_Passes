#include "llvm/ADT/DenseMap.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct Callgraphpass : public CallGraphSCCPass {
  static char ID;
  Callgraphpass() : CallGraphSCCPass(ID) {}

  bool runOnSCC(CallGraphSCC &SC) {
    for (CallGraphSCC::iterator it = SC.begin(); it != SC.end(); it++) {
      Function *F = (*it)->getFunction();
      if (F != nullptr) {

        DenseMap<const char *, int> opcount;
        for (BasicBlock &BB : *F) {
          for (Instruction &I : BB) {
            opcount[I.getOpcodeName()]++;
          }
        }
        errs() << F->getName() << "\n";
        errs() << "============================================================"
                  "=\n";
        errs() << "Operands count: \n";
        for (auto it : opcount) {
          errs() << it.first << ": " << it.second << "\n";
        }
        errs() << "\n=========================================================="
                  "===\n";
      }
    }
    return 1;
  }
};
} // namespace

char Callgraphpass::ID = 0;
static RegisterPass<Callgraphpass> X("callgraph", "CallGraphPass Pass",
                                     false /* Only looks at CFG */,
                                     false /* Analysis Pass */);

static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new Callgraphpass());
                                });