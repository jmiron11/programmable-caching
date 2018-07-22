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

cc_library(
	name = "rocksdb_storage_manager",
	srcs = ["rocksdb_storage_manager.cc"],
	hdrs = ["rocksdb_storage_manager.h"],
	deps = [
		":storage_manager",
		"//external:glog",
		"//proto:storage_service_proto",
		"//storage_service/storage_interfaces:rocksdb_storage_interface",
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "rocksdb_storage_manager_binary",
	srcs = ["rocksdb_storage_manager_binary.cc"],
	deps = [
		":rocksdb_storage_manager",
		"//external:gflags",
		"//external:glog",
	]
)

cc_test(
	name = "rocksdb_storage_manager_test",
	srcs = ["rocksdb_storage_manager_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":rocksdb_storage_manager",
		"//external:gtest",
		"//storage_service/storage_master:storage_master",
		"//proto:storage_service_proto"
	]
)