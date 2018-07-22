load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "execution_engine_interface",
	srcs = ["execution_engine_interface.cc"],
	hdrs = ["execution_engine_interface.h"],
	deps = [
		"//proto:execution_engine_proto",
	] + GRPC_COMPILE_DEPS
)

cc_test(
	name = "execution_engine_interface_test",
	srcs = ["execution_engine_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":execution_engine_interface",
		"//execution_engine/execution_engine:execution_engine",
		"//scheduler/scheduler:scheduler",
		"//storage_service/storage_master:storage_master",
		"//storage_service/storage_client:storage_client",
		"//proto:execution_engine_proto",
		"//external:gtest"
	]
)