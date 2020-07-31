add_llvm_library( Test MODULE
	test.cpp
	InstructionPass.cpp
	OpCounter.cpp
	../Test_Module_Pass/ModuleTest.cpp
	../strongLiveVariableAnalysis/SLVA.cpp
	../Test_CallGraphSCC_Pass/Callgraphpass.cpp
	../Test_Construct_Call_Graph/ConstructCallGraph.cpp
	GetAnalysisInfo.cpp
	GetAsgnVars.cpp
	PLUGIN_TOOL
	opt
)
target_compile_features(Test PRIVATE cxx_range_for cxx_auto_type)
