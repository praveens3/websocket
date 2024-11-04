#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <libwebsockets.h>
#include <string>
#include "utils.h"

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
		std::unique_ptr<ThreadHandler> m_ServerThreadHandler;
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
