load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_manager",
	srcs = ["storage_manager.cc"],
	hdrs = ["storage_manager.h"],
	deps = [
		"//external:glog",
		"//proto:storage_service_proto",
		"//storage_service/rpc_interfaces:storage_master_interface",
		"//storage_service/storage_interfaces:storage_interface",
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "rocksdb_storage_manager_binary",
	srcs = ["rocksdb_storage_manager_binary.cc"],
	deps = [
		":storage_manager",
		"//storage_service/storage_interfaces:storage_interface",
		"//storage_service/storage_interfaces:rocksdb_storage_interface",
		"//external:gflags",
		"//external:glog",
	]
)