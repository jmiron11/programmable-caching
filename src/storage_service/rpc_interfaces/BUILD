load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "storage_client_interface",
	srcs = ["storage_client_interface.cc"],
	hdrs = ["storage_client_interface.h"],
	deps = [
		"//proto:storage_service_proto",
	] + GRPC_COMPILE_DEPS
)

cc_test(
	name = "storage_client_interface_test",
	srcs = ["storage_client_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":storage_client_interface",
		"//storage_service/storage_master:storage_master",
		"//storage_service/storage_client:storage_client",
		"//proto:storage_service_proto",
		"//external:gtest"
	]
)

cc_library(
	name = "storage_master_interface",
	srcs = ["storage_master_interface.cc"],
	hdrs = ["storage_master_interface.h"],
	deps = [
		"//proto:storage_service_proto",
	] + GRPC_COMPILE_DEPS
)

cc_test(
	name = "storage_master_interface_test",
	srcs = ["storage_master_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		":storage_master_interface",
		"//storage_service/storage_master:storage_master",
		"//proto:storage_service_proto",
		"//external:gtest"
	]
)

cc_library(
	name = "storage_manager_interface",
	srcs = ["storage_manager_interface.cc"],
	hdrs = ["storage_manager_interface.h"],
	deps = [
		"//proto:storage_service_proto",
	] + GRPC_COMPILE_DEPS
)

cc_test(
	name = "storage_manager_interface_test",
	srcs = ["storage_manager_interface_test.cc"],
  copts = [
  	"-Iexternal/gtest"
  ],
	deps = [
		":storage_manager_interface",
		"//storage_service/storage_master:storage_master",
		"//storage_service/storage_interfaces:rocksdb_storage_interface",
		"//storage_service/storage_manager:storage_manager",
		"//proto:storage_service_proto",
		"//external:gtest"
	]
)