﻿using System;
using System.Net.WebSockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestApp
{
    public class WebSocketClient
    {
        private ClientWebSocket clientWebSocket;
        private CancellationTokenSource cancellationTokenSource;
        private Action<string> updateViewerAction, updateLogViewerAction;

        public WebSocketClient(Action<string> updateViewer, Action<string> logViewer)
        {
            clientWebSocket = new ClientWebSocket();
            cancellationTokenSource = new CancellationTokenSource();
            updateViewerAction = updateViewer;
            updateLogViewerAction = logViewer;
        }

        public async Task ConnectAsync(Uri serverUri)
        {
            // Load the client certificate
            var clientCertificate = new X509Certificate2("D:\\Users\\PraveenS\\source\\repos\\Github\\Protocol\\src\\certificate\\client_cert.pem");

            // Configure the SSL settings for the WebSocket
            clientWebSocket.Options.ClientCertificates.Add(clientCertificate);

            // Optionally, you can handle SSL validation here if needed (not necessary in most cases)
            clientWebSocket.Options.RemoteCertificateValidationCallback = (sender, cert, chain, sslPolicyErrors) =>
            {
                // Custom certificate validation logic (if needed)
                return sslPolicyErrors == System.Net.Security.SslPolicyErrors.None;
            };

            await clientWebSocket.ConnectAsync(serverUri, CancellationToken.None);
            _ = ReceiveMessagesAsync();
        }

        private async Task ReceiveMessagesAsync()
        {
            var buffer = new byte[1024 * 4]; // 4KB buffer for incoming messages
            while (clientWebSocket.State == WebSocketState.Open)
            {
                var result = await clientWebSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
                if (result.MessageType == WebSocketMessageType.Close)
                {
                    await clientWebSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
                    break;
                }
                else if (result.MessageType == WebSocketMessageType.Text)
                {
                    var receivedMessage = Encoding.UTF8.GetString(buffer, 0, result.Count);
                    updateViewerAction?.Invoke(receivedMessage); // Call the delegate to update the viewer
                    updateLogViewerAction?.Invoke(receivedMessage);
                }
            }
        }

        public async Task SendMessageAsync(string message)
        {
            if (message.Length > 0)
            {
                if (clientWebSocket.State == WebSocketState.Open)
                {
                    var bytes = System.Text.Encoding.UTF8.GetBytes(message);
                    var buffer = new ArraySegment<byte>(bytes);
                    await clientWebSocket.SendAsync(buffer, WebSocketMessageType.Text, true, CancellationToken.None);
                }
            }
        }



        public void Disconnect()
        {
            cancellationTokenSource.Cancel();
            clientWebSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
            updateLogViewerAction?.Invoke("WebSocket connection clossed");
        }
    }
}