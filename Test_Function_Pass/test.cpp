#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"


#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;
namespace {
   struct Test : public FunctionPass {
		static char ID;
		Test() : FunctionPass(ID){}
		bool runOnFunction(Function &F) override{
				errs() << "Test ";
				errs().write_escaped(F.getName()) << "\n";
				return false;
		}	
	};
}

char Test::ID = 0;
static RegisterPass<Test> X("test","Just a test pass",false,false);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new Test()); });
