#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <libwebsockets.h>
#include "HttpHandler.h"

#include <string>

namespace websocket {

	class WebsocketServer : private CHttpHandler {
	public:
		WebsocketServer();
		~WebsocketServer();

		bool initServer(int choice);
		void stopServer();
		void send_heartBeat();



	private:
		struct lws_context* m_LWSContext;
		static struct lws_protocols m_Protocols[];
		std::unique_ptr<ThreadHandler> m_ServerThreadHandler;
		uint64_t m_LastHBtime = 0;
		Client m_Clients;

		bool initDefaultWSServer();
		bool initHTTP2WSServer();
		void run();
		bool sendMsg(struct lws* wsi, const std::string& data, bool logEnabled = true);

		//utils
		static WebsocketServer* getServer(struct lws* wsi);
		static std::string getClientinfo(struct lws* wsi);
		static Client::DataMap* getClientDataMap(struct lws* wsi);
		static int callback_wss(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);
		static int callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);
	};

} // namespace websocket

#endif // WEBSOCKET_SERVER_H
