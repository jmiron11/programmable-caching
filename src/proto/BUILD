load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_library")
load("@com_github_grpc_grpc//bazel:grpc_build_system.bzl", "grpc_proto_library")


package(default_visibility = ["//visibility:public"])

#grpc_proto_library(
#	name = "storage_service_proto",
#	srcs = [
#		"storage_service.proto",	
#	],
#	generate_mocks=True
#)

cpp_proto_library(
	name = "storage_service_proto",
	protos = [
		"storage_service.proto",
	],
	with_grpc=True,
)