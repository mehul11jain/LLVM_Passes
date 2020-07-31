#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
namespace {
   struct GetAsgnVarsPass : public FunctionPass {
		static char ID;
		GetAsgnVarsPass() : FunctionPass(ID){}
		bool runOnFunction(Function &F) override{				
                for(BasicBlock& BB : F)
                {
                    for(Instruction& I : BB)
                    {
                       // result 
                        
                        // outs()<<"LHS: "<<res->getName();
                        //operands       
                        errs() << "RHS: {";                
                        for(User::op_iterator it=I.op_begin();it!=I.op_end();it++)
                        {
                            if(it!=I.op_begin())
                                errs() <<", ";
                            errs() << it->get()->getName();
                        }                  
                        errs()<<" }\n";   
                    }
                }				
		return false;
		}	
	};
}

char GetAsgnVarsPass::ID = 0;
static RegisterPass<GetAsgnVarsPass> X("asgnvars","Get variables in an assignment",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new GetAsgnVarsPass()); });
