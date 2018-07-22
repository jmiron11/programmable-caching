load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_client",
	srcs = ["storage_client.cc"],
	hdrs = ["storage_client.h"],
	deps = [
		"//external:glog",
		"//storage_service/rpc_interfaces:storage_master_interface",
		"//proto:storage_service_proto",
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "storage_client_binary",
	srcs = ["storage_client_binary.cc"],
	deps = [
		":storage_client",
		"//external:gflags",
	]
)