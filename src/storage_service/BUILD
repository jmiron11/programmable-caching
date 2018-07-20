load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")

package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_master",
	hdrs = ["storage_master.h"],
	srcs = ["storage_master.cc"],
	deps = [
		":storage_manager_view",
		":storage_file_view",
		"//proto:storage_service_proto",
		"//external:glog",
	] + GRPC_COMPILE_DEPS
)

cc_library(
	name = "storage_manager",
	srcs = ["storage_manager.cc"],
	hdrs = ["storage_manager.h"],
	deps = [
		":storage_interface",
		":storage_manager_view",
		"//proto:storage_service_proto",
		"//external:glog",
	] + GRPC_COMPILE_DEPS
)

cc_library(
	name = "rocksdb_storage_manager",
	srcs = ["rocksdb_storage_manager.cc"],
	hdrs = ["rocksdb_storage_manager.h"],
	deps = [
		":storage_manager",
		":rocksdb_storage_interface",
		"//external:glog",
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

cc_library(
	name = "storage_interface",
	hdrs = ["storage_interface.h"],
	deps = [
		"//proto:storage_service_proto",
	]
)

cc_library(
	name = "rocksdb_storage_interface",
	hdrs = ["rocksdb_storage_interface.h"],
	srcs = ["rocksdb_storage_interface.cc"],
	deps = [
		":storage_interface",
		"//external:rocksdb",	
		"//proto:storage_service_proto"
	]
)

cc_test(
	name = "rocksdb_storage_manager_test",
	srcs = ["rocksdb_storage_manager_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		"//external:gtest",
		":rocksdb_storage_manager"
	]
)

cc_test(
	name = "rocksdb_storage_interface_test",
	srcs = ["rocksdb_storage_interface_test.cc"],
  copts = ["-Iexternal/gtest"],
	deps = [
		"//external:gtest",
		":storage_interface",
		":rocksdb_storage_interface",
		"//external:rocksdb"	
	]
)

cc_library(
	name = "s3_storage_interface",
	hdrs = ["s3_storage_interface.h"],
	srcs = ["s3_storage_interface.cc"],
  copts = [
  	"-laws-cpp-sdk-s3"
  ],
	deps = [
		":storage_interface",
	]
)

cc_test(
	name = "s3_storage_interface_test",
	srcs = ["s3_storage_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		"//external:gtest",
		":storage_interface",
		":s3_storage_interface",
	]
)

cc_library(
	name = "storage_manager_view",
	hdrs = ["storage_manager_view.h"],
	srcs = ["storage_manager_view.cc"],
	deps = [
		"//proto:storage_service_proto",
	] + GRPC_COMPILE_DEPS
)

cc_library(
	name = "storage_file_view",
	hdrs = ["storage_file_view.h"],
	srcs = ["storage_file_view.cc"]
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