load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "execution_engine",
	srcs = ["execution_engine.cc"],
	hdrs = ["execution_engine.h"],
	deps = [
		"//storage_service/rpc_interfaces:storage_client_interface",
		"//scheduler/rpc_interfaces:scheduler_interface",
		"//external:glog",
		"//proto:execution_engine_proto",
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "execution_engine_binary",
	srcs = ["execution_engine_binary.cc"],
	deps = [
		":execution_engine"
	]
)