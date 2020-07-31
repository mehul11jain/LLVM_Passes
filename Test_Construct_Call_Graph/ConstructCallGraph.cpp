#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/Twine.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <set>
using namespace llvm;
namespace {
class ConstructGraph {
  std::set<std::string> edges;
  raw_fd_ostream *file;
  std::error_code EC;

public:
  ConstructGraph() { file = new raw_fd_ostream("Call_Graph", EC); }
  raw_fd_ostream *get_stream() { return file; }
  std::set<std::string> get_edges() { return edges; }
  void WriteToDot(Function &F) {
    if (!EC) {
      StringRef current_func = F.getName();
      for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
          if (const CallInst *callinst = dyn_cast<CallInst>(&I)) {
            StringRef called_func = callinst->getCalledFunction()->getName();
            Twine res = current_func + "->" + called_func + ";\n";
            edges.insert(res.str());
          }
        }
      }
    }
  }
};
struct ConstructCallGraph : public ModulePass {
  static char ID;
  ConstructCallGraph() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    ConstructGraph *C = new ConstructGraph();
    *(C->get_stream()) << "digraph {\n";
    Module::FunctionListType &flt = M.getFunctionList();
    Module::iterator i;
    for (i = flt.begin(); i != flt.end(); i++) {
      C->WriteToDot(*i);
    }
    for (auto x : C->get_edges()) {
      *(C->get_stream()) << x;
    }
    *(C->get_stream()) << " }\n";
    (C->get_stream())->close();

    return false;
  }
};
} // namespace

char ConstructCallGraph::ID = 0;
static RegisterPass<ConstructCallGraph>
    X("cgraph", "prints call graph in a dot file", false, false);

static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                [](const PassManagerBuilder &Builder,
                                   legacy::PassManagerBase &PM) {
                                  PM.add(new ConstructCallGraph());
                                });
