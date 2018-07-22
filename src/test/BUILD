load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")
package(default_visibility = ["//visibility:public"])

cc_test(
	name = "maestro_test",
	srcs = ["maestro_test.cc"],
  copts = [
  	"-Iexternal/gtest",
  ],
	deps = [
		"//scheduler/rpc_interfaces:scheduler_interface",
		"//execution_engine/execution_engine:execution_engine",
		"//scheduler/scheduler:scheduler",
		"//storage_service/storage_master:storage_master",
		"//storage_service/storage_manager:storage_manager",
		"//storage_service/storage_interfaces:rocksdb_storage_interface",
		"//storage_service/storage_client:storage_client",
		"//proto:execution_engine_proto",
		"//external:gtest"
	]
)