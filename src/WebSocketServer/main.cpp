#include <iostream>
#include "server.h"  // Include your WebsocketServer header

int main(int argc, char* argv[]) {
    initLog(logSection);  // Initialize logging with the specified section

    // Check if an argument was provided
    if (argc < 2) {
        LOG_ERROR("Usage: " + std::string(argv[0]) + " <server_type>\n"
            "<server_type>: 1 for Default WebSocket (ws://localhost:8080), 2 for HTTP/2 WebSocket (wss://localhost:8443)");
        return -1;
    }

    // Create an instance of WebsocketServer
    websocket::WebsocketServer wss;

    int choice = std::stoi(argv[1]); // Convert the command line argument to an integer
    wss.initServer(choice);

    std::string input;
    bool running = true;
    while (running) {
        // Display command prompt
        std::cout << "\nEnter command (start, stop, exit): ";
        std::getline(std::cin, input);

        // Process the command
        if (input == "start") {
            LOG_INFO("Starting server...");
            wss.initServer(choice);
        }
        else if (input == "stop") {
            LOG_INFO("Stopping server...");
            wss.stopServer();
        }
        else if (input == "exit") {
            LOG_INFO("Exiting program...");
            wss.stopServer();  // Ensure the server is stopped before exiting
            running = false;
        }
        else {
            std::cout << "Invalid command. Please enter 'start', 'stop', or 'exit'.\n";
        }
        }
    return 0;
}
