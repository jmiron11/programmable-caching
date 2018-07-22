load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

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
	name = "redis_storage_interface",
	hdrs = ["redis_storage_interface.h"],
	srcs = ["redis_storage_interface.cc"],
	deps = [
		":storage_interface",
		"//proto:storage_service_proto",
		"//external:hiredis"
	]
)

cc_test(
	name = "redis_storage_interface_test",
	srcs = ["redis_storage_interface_test.cc"],
  copts = ["-Iexternal/gtest"],
	deps = [
		":storage_interface",
		":redis_storage_interface",
		"//external:glog",
		"//external:gtest"
	]
)
