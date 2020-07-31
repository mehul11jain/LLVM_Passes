#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
namespace {
   struct OpCounterPass : public FunctionPass {
		static char ID;
      StringMap<int> opcodemap;
		OpCounterPass() : FunctionPass(ID){}

      bool runOnFunction(Function& F) override{
         for(BasicBlock& BB : F)
         {
            for(Instruction& I : BB)
            {
               if(opcodemap.find(I.getOpcodeName())==opcodemap.end())
                  opcodemap[I.getOpcodeName()]=1;
               else
                  opcodemap[I.getOpcodeName()]+=1;
            }
         }
         StringMapIterator<int> start = opcodemap.begin();
         StringMapIterator<int> end = opcodemap.end();
	 int cnt = 0;
         while(start!=end)
         {
            errs() <<++cnt<<". "<<(*start).getKey() << "- "<<(*start).getValue()<<"\n";
            start++;
         }
         return false;
      }
			
	};
}

char OpCounterPass::ID = 0;
static RegisterPass<OpCounterPass> X("opcount","COunt of each operation in the IR",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new OpCounterPass()); });
