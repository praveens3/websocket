#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <libwebsockets.h>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Log.h"

struct ThreadHandler {
	std::atomic<bool> m_KeepAlive;
	std::mutex m_Mutex;
	std::condition_variable m_CV;
	std::unique_ptr<std::thread> m_ThreadPtr;

	ThreadHandler(std::unique_ptr<std::thread> thread) : m_ThreadPtr(std::move(thread)), m_KeepAlive(true) {}
	ThreadHandler(const ThreadHandler&) = delete;
	ThreadHandler& operator=(const ThreadHandler&) = delete;

	~ThreadHandler() {
		m_KeepAlive = false;
		m_CV.notify_one();
		join();
	}
	void join() {
		if (m_ThreadPtr && m_ThreadPtr->joinable()) {
			m_ThreadPtr->join();
		}
	}
};

struct Client {
private:
	struct DataMap {
		struct lws* m_Data;
		uint64_t m_Seq;
		DataMap(struct lws* d) : m_Data(d), m_Seq(0) {}
	};
	std::unordered_map<std::string, DataMap> m_ClientMap;

public:

	void addClient(const std::string& clientId, struct lws* wsi) {
		if (!m_ClientMap.count(clientId)) {
			m_ClientMap.emplace(clientId, DataMap(wsi));
			LOG_INFO("added new client: {}, count: {}", clientId.c_str(), m_ClientMap.size());
		}
	}
	void removeClient(const std::string& clientId) {
		if (m_ClientMap.count(clientId)) {
			m_ClientMap.erase(clientId);
			LOG_INFO("removed client: {}, count: {}", clientId.c_str(), m_ClientMap.size());
		}
	}

	void removeall() {
		m_ClientMap.empty();
	}
	
	struct lws* getClient(const std::string& clientId) {
		auto dataMap = getData(clientId);
		if (dataMap) {
			return dataMap->m_Data;
		}
		return nullptr;
	}

	uint64_t getSeq(const std::string& clientId) {
		auto dataMap = getData(clientId, false);
		if (dataMap) {
			return dataMap->m_Seq;
		}
		return 0;
	}

	void incSeq(const std::string& clientId) {
		auto dataMap = getData(clientId, false);
		if (dataMap) {
			dataMap->m_Seq++;
		}
	}

	DataMap* getData(const std::string& clientId, bool logEnabled = true) {
		if (m_ClientMap.count(clientId)) {
			auto itt = m_ClientMap.find(clientId);
			if (itt != m_ClientMap.end()) {
				if(logEnabled)
					LOG_INFO("found client: {}, count: {}", clientId.c_str(), m_ClientMap.size());
				return &(itt->second);
			}
			else {
				if(logEnabled)
					LOG_ERROR("client not found, ID: {}", clientId.c_str());
				return nullptr;
			}
		}
	}

	const std::unordered_map<std::string, DataMap>& data() const {
		return m_ClientMap;
	}


};

namespace websocket {

	class WebsocketServer {
	public:
		WebsocketServer();
		~WebsocketServer();

		bool initServer(int choice);
		void stopServer();
		void send_heartBeat();



	private:
		struct lws_context* m_LWSContext;
		static struct lws_protocols m_Protocols[];
		std::unique_ptr<ThreadHandler> m_ServerThreadHandler, m_HeartBeatThreadHandler;
		uint64_t m_LastHBtime = 0;
		Client m_Clients;

		bool initDefaultWSServer();
		bool initHTTP2WSServer();
		void run();
		bool sendMsg(struct lws* wsi, const std::string& data, bool logEnabled = true);

		static int callback_wss(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);
	};

} // namespace websocket

#endif // WEBSOCKET_SERVER_H
