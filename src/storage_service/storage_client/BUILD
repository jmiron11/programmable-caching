load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_client",
	srcs = ["storage_client.cc"],
	hdrs = ["storage_client.h"],
	deps = [
		":rule_db",
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

cc_library(
	name = "rule_db",
	srcs = ["rule_db.cc"],
	hdrs = ["rule_db.h"],
	deps = [
		"//proto:storage_service_proto"
	]
)

cc_test (
	name = "rule_db_test",
	srcs = ["rule_db_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":rule_db",
		"//external:gtest",
		"//proto:storage_service_proto"
	]
)