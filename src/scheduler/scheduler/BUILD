load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "scheduler",
	srcs = ["scheduler.cc"],
	hdrs = ["scheduler.h"],
	deps = [
		"//external:glog",
		"//storage_service/rpc_interfaces:storage_master_interface",
		"//proto:scheduler_proto",
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "basic_scheduler_binary",
	srcs = ["basic_scheduler_binary.cc"],
	deps = [
		":scheduler"
	]
)