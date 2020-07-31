#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/SymbolTableListTraits.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct ModuleTest : public ModulePass {
  static char ID;
  ModuleTest() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    Module::FunctionListType& flt = M.getFunctionList();
    Module::iterator i;
    for(i=flt.begin();i!=flt.end();i++)
	{
		errs() << "Hello: ";
		errs()<<(*i).getName()<<"\n";
	}
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char ModuleTest::ID = 0;
static RegisterPass<ModuleTest> X("Moduletest", "ModulePass Test",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new ModuleTest()); });
