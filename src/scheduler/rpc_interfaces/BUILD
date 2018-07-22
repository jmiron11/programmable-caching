load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "scheduler_interface",
	srcs = ["scheduler_interface.cc"],
	hdrs = ["scheduler_interface.h"],
	deps = [
		"//proto:scheduler_proto",
	] + GRPC_COMPILE_DEPS
)

cc_test(
	name = "scheduler_interface_test",
	srcs = ["scheduler_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":scheduler_interface",
		"//scheduler/scheduler:scheduler",
		"//storage_service/storage_master:storage_master",
		"//proto:scheduler_proto",
		"//external:gtest"
	]
)