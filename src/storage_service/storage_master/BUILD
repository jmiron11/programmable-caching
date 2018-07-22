load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_master",
	hdrs = ["storage_master.h"],
	srcs = ["storage_master.cc"],
	deps = [
		"//external:glog",
		"//proto:storage_service_proto",	
		"//storage_service/rpc_interfaces:storage_client_interface"
		] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "storage_master_binary",
	srcs = ["storage_master_binary.cc"],
	deps = [
		":storage_master",
		"//external:gflags",
		"//external:glog",
	]
)

cc_test(
	name = "storage_master_test",
	srcs = ["storage_master_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		"//storage_service/storage_client:storage_client",
		"//storage_service/rpc_interfaces:storage_master_interface",
		":storage_master",
		"//proto:storage_service_proto",
		"//external:gtest"
	]
)