using System;
using WebSocketSharp;

class TestApp
{
    static void Main(string[] args)
    {
        using (var ws = new WebSocket("ws://localhost:8080"))
        {
            ws.OnMessage += (sender, e) => Console.WriteLine("Received: " + e.Data);

            ws.Connect();
            ws.Send("Hello, Server!");

            Console.ReadKey(true); // Wait for user input to close the app
        }
    }
}
