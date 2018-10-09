# Programmable Caching

End-to-end cluster management system for storage-separated sytsems with pluggable scheduling and data movement policies. Data movement is accomplised through programmable caching (similar to programmable switches). Cluster operators may specify data movement rules that compose a caching policy and determine where to read inputs and cache inputs when jobs are scheduled.

# Dependencies
 * protobuf
 * gRPC
 * googletest
 * glog

Maestro uses the bazel build system to retrieve dependencies. 
Bazel installation instructions can be found [here](https://docs.bazel.build/versions/master/install.html).


# Pre-requisites
libcurl4-openssl-dev libssl-dev uuid-dev zlib1g-dev libpulse-dev

Setup Amazon C++ SDK: [Setup](https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html)
