load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")

package(default_visibility = ["//visibility:public"])

cc_binary(
	name = "storage_master",
	srcs = ["storage_master.cc"],
	deps = [
	"//proto:storage_service_proto",
	"@glog//:glog"
	] + GRPC_COMPILE_DEPS
)

cc_library(
	name = "storage_manager",
	srcs = ["storage_manager.cc"],
	hdrs = ["storage_manager.h"],
	deps = [
	":storage_interface",
	"//proto:storage_service_proto",
		"@glog//:glog"
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "storage_manager_binary",
	srcs = ["storage_manager_binary.cc"],
	deps = [
		":storage_manager",
		"@gflags//:gflags",
		"@glog//:glog"
	]
)

cc_library(
	name = "storage_interface",
	hdrs = ["storage_interface.h"]
)

cc_test(
	name = "storage_manager_test",
	srcs = ["storage_manager_test.cc"],
  copts = ["-Iexternal/gtest"],
	deps = [
		"@gtest//:gtest_main"
	]
)