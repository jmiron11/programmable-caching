cc_test(
	name = "storage_service_test",
	srcs = ["storage_service_test.cc"],
  copts = [
  	"-Iexternal/gtest"
  ],
	deps = [
		"//storage_service/rpc_interfaces:storage_master_interface",
		"//storage_service/rpc_interfaces:storage_client_interface",
		"//storage_service/storage_client:storage_client",
		"//storage_service/storage_master:storage_master",
		"//storage_service/storage_manager:storage_manager",
		"//storage_service/storage_interfaces:rocksdb_storage_interface",
		"//storage_service/storage_interfaces:storage_interface",
		"//proto:storage_service_proto",
		"//external:gtest"
	]
)