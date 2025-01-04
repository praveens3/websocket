#include "server.h"
#include <cstring>

using namespace websocket;

#define log_instance "webSocketServer"

const uint16_t SERVER_LOOP_WAIT_TIME_MS = 100;
const uint16_t SEND_PERIODIC_MSG_INTERVAL_MS = 5000;

struct lws_protocols WebsocketServer::m_Protocols[] = {
	{
		"ws",								// Protocol name
		WebsocketServer::callback_wss,              // Callback function for this protocol
		sizeof(WebsocketServer*),                   // User data per connection
		1024,										// Max frame size
	},
	//{
	//	"http",                                     // Protocol name for HTTP
	//	WebsocketServer::callback_http,             // Callback function for HTTP protocol
	//	0,                                          // No user data needed for HTTP
	//	0,                                          // Max frame size not applicable to HTTP
	//},
	{ NULL, NULL, 0, 0 }							// Terminator
};

WebsocketServer::WebsocketServer() : m_LWSContext(nullptr) {
	initLog(log_instance);
}

WebsocketServer::~WebsocketServer() {
	stopServer();
}

bool WebsocketServer::initServer(int choice) {
	bool retVal = false;
	if (choice == 1) {
		retVal = initDefaultWSServer();
	}
	else if (choice == 2) {
		retVal = initHTTP2WSServer();
	}
	else {
		LOG_ERROR("Invalid choice for WebSocket server initialization.");
	}

	if (retVal) {
		m_ServerThreadHandler.reset(new ThreadHandler(std::make_unique<std::thread>(&WebsocketServer::run, this)));
	}
	return retVal;
}

bool WebsocketServer::initDefaultWSServer() {
	lws_set_log_level(15, lwsl_emit_stderr);
	LOG_INFO("Log level set to verbose\n");

	struct lws_context_creation_info context_info;
	memset(&context_info, 0, sizeof(context_info));

	context_info.port = 7002; // Port for Default WS
	context_info.protocols = m_Protocols; // Supported m_Protocols
	context_info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE
		| LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT | LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT;

	// Set paths for the SSL certificate and private key
	context_info.ssl_cert_filepath = "D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\server_cert.pem"; // SSL certificate
	context_info.ssl_private_key_filepath = "D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\server_key.pem"; // SSL private key
	context_info.ssl_ca_filepath = "D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\ca_cert.pem";  // Root CA for verifying client certificates
	//context_info.provided_client_ssl_ctx = SSL_VERIFY_PEER;  // Request client certificates and verify them
	context_info.user = this;

	// Create the WS context
	m_LWSContext = lws_create_context(&context_info);
	if (!m_LWSContext) {
		LOG_ERROR("Failed to create WSS context with SSL");
		return false;
	}

	LOG_INFO("Default WS server started on port 8080");

	return true;
}

bool WebsocketServer::initHTTP2WSServer() {
	struct lws_context_creation_info context_info;
	memset(&context_info, 0, sizeof(context_info));

	context_info.port = 8443; // Port for HTTP/2 WS
	context_info.protocols = m_Protocols; // Supported m_Protocols
	context_info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE
		| LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

	// Set paths for the SSL certificate and private key
	context_info.ssl_cert_filepath = "D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\server_cert.pem"; // SSL certificate
	context_info.ssl_private_key_filepath = "D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\server_key.pem"; // SSL private key
	context_info.user = this;
	
	// Create the WS context
	m_LWSContext = lws_create_context(&context_info);
	if (!m_LWSContext) {
		LOG_ERROR("Failed to create HTTP/2 WSS context");
		return false;
	}

	LOG_INFO("HTTP/2 WS server started on port 8443");
	return true;
}

void WebsocketServer::run() {
	LOG_INFO("Server thread start running");

	std::unique_lock<std::mutex> ul(m_ServerThreadHandler->m_Mutex);
	while (m_ServerThreadHandler && m_ServerThreadHandler->m_KeepAlive.load()) {
		if (m_ServerThreadHandler->m_CV.wait_for(ul, std::chrono::milliseconds(SERVER_LOOP_WAIT_TIME_MS), [this] { return !m_ServerThreadHandler->m_KeepAlive.load(); })) {
			LOG_INFO("Server thread signed to stop");
			break;
		}
		else {
			if (m_LWSContext) {
				int result = lws_service(m_LWSContext, 1000);
				if (result < 0) {
					LOG_ERROR("lws_service encountered an error: " + std::to_string(result));
					// Optionally handle the error, e.g., by shutting down or reinitializing
				}
			}
			else {
				LOG_ERROR("LWS Context is null, cannot call lws_service.");
			}
			send_heartBeat();
		}
	}
	LOG_INFO("server thread stopped");
}

void WebsocketServer::stopServer() {
	if(m_ServerThreadHandler)
		m_ServerThreadHandler.reset(nullptr);

	if (m_LWSContext) {
		lws_context_destroy(m_LWSContext);
		m_LWSContext = nullptr;
	}
	LOG_INFO("WebSocket server stopped.");
}

void WebsocketServer::send_heartBeat() {
	
	if (m_Clients.data().size() && (GetTickCount64() - m_LastHBtime > SEND_PERIODIC_MSG_INTERVAL_MS))
	{
		for (const auto& d : m_Clients.data()) {
			if (d.second.m_Type)
			{
				m_Clients.incSeq(d.first);
				sendMsg(d.second.m_Data, "Heartbeat: " + std::to_string((d.second.m_Seq)), false);
				m_LastHBtime = GetTickCount64();
			}
		}
	}
}

bool WebsocketServer::sendMsg(struct lws* wsi, const std::string& data, bool logEnabled) {
	std::vector<unsigned char> buf(LWS_PRE + data.size());
	std::memcpy(&buf[LWS_PRE], data.c_str(), data.size());
	const ssize_t written = lws_write(wsi, &buf[LWS_PRE], data.size(), LWS_WRITE_TEXT);

	if (logEnabled) {
		if (written < 0) {
			LOG_ERROR("Error sending message to client, msg size: " + std::to_string(written));
			return false;
		}
		else {
			LOG_INFO("Message sent to client successfully: {}, len: {}, msg size: {}", data.c_str(), written, data.size());
		}
	}
	return true;
}

int WebsocketServer::callback_wss(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
	switch (reason) {
		case LWS_CALLBACK_ESTABLISHED: {
			LOG_INFO("WS connection established");
			WebsocketServer* server = getServer(wsi);
			if (server) {
				const std::string clientInfo = getClientinfo(wsi);
				server->m_Clients.addClient(clientInfo, wsi);
				LOG_INFO("Added new WS client [{}] to server, count: {}", clientInfo, server->m_Clients.data().size());
			}
			break;
		}

		case LWS_CALLBACK_RECEIVE: {
			std::string received_message((char*)in, len);  // Convert the received message to a string
			LOG_INFO("WS received message: " + received_message);
			WebsocketServer* server = getServer(wsi);
			if (server) {
				server->sendMsg(wsi, "Resp: " + received_message);
			}
			break;
		}

		case LWS_CALLBACK_CLOSED: {
			LOG_INFO("WS connection closed");
			WebsocketServer* server = getServer(wsi);
			if (server) {
				const std::string clientInfo = getClientinfo(wsi);
				server->m_Clients.removeClient(clientInfo);
				LOG_INFO("Removed WS client [{}] from  server, count: {}", clientInfo, server->m_Clients.data().size());
			}
		}
			break;

		default:
			callback_http(wsi, reason, user, in, len);
			break;
	}
	return 0;
}

int WebsocketServer::callback_http(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
	switch (reason) {
	case LWS_CALLBACK_HTTP:
		LOG_INFO("Http message received");
		if (lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI)) {
			const std::string clientInfo = getClientinfo(wsi);
			WebsocketServer* server = getServer(wsi);
			if (server) {
				server->m_Clients.addClient(clientInfo, wsi, 0);
				LOG_INFO("Http file received, Client Id: {}, count: {}", clientInfo.c_str(), server->m_Clients.data().size());
			}
		}
		break;

	case LWS_CALLBACK_HTTP_BODY: {
		std::string buffer((const char*)in, len);
		handleFileWrite(buffer, getClientDataMap(wsi));
		break;
	}

	case LWS_CALLBACK_HTTP_BODY_COMPLETION: {
		const std::string clientInfo = getClientinfo(wsi);
		auto dataMap = getClientDataMap(wsi);
		if (dataMap)
		{
			dataMap->outfile.close();
			dataMap->file_write_inProgress = false;
			if (validateFileChecksum(dataMap)) {
				LOG_INFO("Http File download completed. filename: {}, size: {}, client: {} ", dataMap->filename.c_str(), dataMap->filesize, clientInfo.c_str());
				lws_return_http_status(wsi, HTTP_STATUS_OK, nullptr);
			}
			else {
				LOG_WARN("Http File download completed, but checksum not matched, filename: {}, size: {}, client: {} ", dataMap->filename.c_str(), dataMap->filesize, clientInfo.c_str());
				lws_return_http_status(wsi, HTTP_STATUS_PARTIAL_CONTENT, nullptr);
			}
		} else {
			lws_return_http_status(wsi, HTTP_STATUS_INTERNAL_SERVER_ERROR, nullptr);
		}
		lws_http_transaction_completed(wsi);

		LOG_INFO("Http client connection closed");
		WebsocketServer* server = getServer(wsi);
		if (server) {
			server->m_Clients.removeClient(clientInfo);
			LOG_INFO("Removed http client [{}] from server, count: {}", clientInfo, server->m_Clients.data().size());
		}
	}
	break;

	default:
		break;
	}
	
	return 0;
}

WebsocketServer* WebsocketServer::getServer(struct lws* wsi) {
	WebsocketServer* server = nullptr;
	auto context = lws_get_context(wsi);
	if (context) {
		auto userdata = lws_context_user(context);
		if (userdata) {
			server = reinterpret_cast<WebsocketServer*>(userdata);
		}
	}
	return server;
}

Client::DataMap* WebsocketServer::getClientDataMap(struct lws* wsi) {
	const std::string clientInfo = getClientinfo(wsi);
	WebsocketServer* server = getServer(wsi);
	if (server)
	{
		auto data = server->m_Clients.getData(clientInfo, false);
		return data;
	}
	return nullptr;
}

std::string WebsocketServer::getClientinfo(struct lws* wsi) {
	std::string clientInfo;
	char clientIp[128];
	if (lws_get_peer_simple(wsi, clientIp, sizeof(clientIp))) {
		std::ostringstream oss;
		oss << std::hex << wsi;
		clientInfo = clientIp + std::string("-") + oss.str() + "-" + std::to_string(lws_get_socket_fd(wsi));
		LOG_DEBUG("Assigning lws id to wss server name: ", clientInfo);
	}
	return clientInfo;
}