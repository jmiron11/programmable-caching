
#include "storage_interface.h"
#include "s3_storage_interface.h"

#include <glog/logging.h>

// #include <aws/core/Aws.h>
// #include <aws/s3/S3Client.h>
// #include <aws/s3/model/CreateBucketRequest.h>

S3StorageInterface::S3StorageInterface(std::string bucket_name) {
	// Aws::SDKOptions options;
	// Aws::InitAPI(options);

	// Aws::S3::Model::CreateBucketRequest request;
	// request.SetBucket(Aws::Utils::StringUtils::to_string<std::string>(bucket_name));

	// Aws::S3::S3Client s3_client;
	// auto outcome = s3_client.CreateBucket(request);

	// if (outcome.IsSuccess()) {
	// 	std::cout << "Done!" << std::endl;
	// } else {
	// 	std::cout << "CreateBucket error: "
	// 	          << outcome.GetError().GetExceptionName() << std::endl
	// 	          << outcome.GetError().GetMessage() << std::endl;
	// }
}

S3StorageInterface::~S3StorageInterface() {
	// Aws::SDKOptions options;
	// Aws::ShutdownAPI(options);

}

StorageInterface::Status S3StorageInterface::Get(const std::string& key,
    std::string* value) {
	return StorageInterface::OK;
}

StorageInterface::Status S3StorageInterface::Put(const std::string& key,
    const std::string& value) {
	return StorageInterface::OK;
}

