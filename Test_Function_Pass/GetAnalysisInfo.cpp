#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/Analysis/LoopInfo.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
namespace {
   struct GetAnalysisInfo : public FunctionPass {
		static char ID;
		GetAnalysisInfo() : FunctionPass(ID){}
    void getAnalysisUsage(AnalysisUsage &AU) const{
        AU.setPreservesAll();
        AU.addRequired<LoopInfoWrapperPass>();
    }

    void countBlocksInLoop(Loop* L,unsigned nest)
    {
        unsigned num_Blocks = 0;
        Loop::block_iterator bb;
        for (bb = L->block_begin(); bb != L->block_end(); ++bb)
          num_Blocks++;
        errs() << "Loop level " << nest << " has " << num_Blocks
              << " blocks\n";
        std::vector<Loop *> subLoops = L->getSubLoops();
        Loop::iterator j, f;
        for (j = subLoops.begin(), f = subLoops.end(); j != f; ++j)
          countBlocksInLoop(*j, nest + 1); 
    }
		virtual bool runOnFunction(Function &F){				
                LoopInfo* LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();				
                outs() << " Function " << F.getName() << "\n";
                for(Loop *L : *LI)
                    countBlocksInLoop(L,0);
				return false;
		}	
	};
}

char GetAnalysisInfo::ID = 0;
static RegisterPass<GetAnalysisInfo> X("getanalysisinfo","gets info from other pass",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new GetAnalysisInfo()); });

