#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/User.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
namespace {
   struct InstructionPass : public FunctionPass {
		static char ID;
		InstructionPass() : FunctionPass(ID){}
		bool runOnFunction(Function &F) override{				
                for(BasicBlock& BB : F)
                {
                    for(Instruction& I : BB)
                    {                           
                        errs()<<I<<"\n";    
                    }
                }				
		return false;
		}	
	};
}

char InstructionPass::ID = 0;
static RegisterPass<InstructionPass> X("instruction","Iterates over each instruction",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new InstructionPass()); });
