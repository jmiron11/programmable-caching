load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")

package(default_visibility = ["//visibility:public"])

cc_binary(
	name = "storage_master",
	srcs = ["storage_master.cc"],
	deps = [
	"//proto:storage_service_proto"
	] + GRPC_COMPILE_DEPS
)

cc_binary(
	name = "storage_manager",
	srcs = ["storage_manager.cc"],
	deps = [
	"//proto:storage_service_proto"
	] + GRPC_COMPILE_DEPS
)