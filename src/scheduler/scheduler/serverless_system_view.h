#ifndef SERVERLESS_SYSTEM_VIEW_
#define SERVERLESS_SYSTEM_VIEW_

#include "proto/scheduler.grpc.pb.h"
#include "proto/storage_service.grpc.pb.h"

#include <vector>
#include <string>

// Presents a view of the storage service as a two layer hierarchy.
//
// 			  Persistent Storage
//  					     |
//			-----------------------
//		Eph   	Eph	  	Eph	  	Eph
//		 |		   |	  	 |	  	 |
//	Client  Client	Client	Client
//     |			 |			 |			 |
//	 Exec		 Exec		 Exec		 Exec
//


class ServerlessSystemView {
 public:
	ServerlessSystemView(StorageName ephemeral, int eph_cost, StorageName persistent,
	                      int persistent_cost);

	/* System view management API. */

	/* Functions that construct the serverless system view */
	// Add execution engine
	void AddExecutionEngine(const std::string& name, const std::string client_uri);

	// Changes the storage file view based off of the received GetView.
	void ParseGetViewReply(const GetViewReply& master_view);

 	// Returns a mapping of client to files, this allows scheduler to know what 
	// files are cached. Assume if not cached, it is in persistent.
	std::vector<std::string> GetEphemeralStorageView(const std::string& engine_name);

	/* Scheduler API. */

	// Returns the ephemeral manager from client, allows scheduler to 
	// make rules.
	std::string GetEphemeralMgrFromEngine(const std::string& engine_name);

	// Returns the persistent manager that has a file. Allows scheduler
	// to make rules for retrieval from persistent storage.
	std::string GetPersistentMgrForFile(const std::string& file_name);
};

#endif