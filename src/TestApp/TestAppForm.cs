using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestApp
{
    public partial class TestAppForm : Form
    {
        private WebSocketClient webSocketClient;
        private bool viewHeartBeat = false;
        public TestAppForm()
        {
            InitializeComponent();
            webSocketClient = new WebSocketClient(UpdateViewerBox);
        }

        private async void connectServer()
        {
            try
            {
                Uri serverUri = new Uri("wss://localhost:8080");
                await webSocketClient.ConnectAsync(serverUri);
                statusLabel.Text = "Connected to WebSocket server!";
            }
            catch (Exception ex)
            {
                statusLabel.Text = $"Error connecting to server: " + ex.Message;
                connectButton.Text = "Connect";
                connectButton.BackColor = Color.Aquamarine;
            }
        }
        private void TestAppUI_FormClosing(object sender, FormClosingEventArgs e)
        {
            webSocketClient.Disconnect();
            statusLabel.Text = "Server disconnected";
        }

        public void UpdateViewerBox(string message)
        {
            if (!viewHeartBeat && message.Contains("Heartbeat:"))
                return;

            if (viewerBox.InvokeRequired)
            {
                viewerBox.Invoke(new Action(() => viewerBox.AppendText($"{message}\n"))); // Append message to RichTextBox
            }
            else
            {
                viewerBox.AppendText($"{message}\n"); // Append message to RichTextBox
            }
        }

        private async void sendButton_Click(object sender, EventArgs e)
        {
            await webSocketClient.SendMessageAsync(editorBox.Text);
        }

        private void checkedListBox1_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            bool isChecked = e.NewValue == CheckState.Checked;

            switch (e.Index)
            {
                case 0:
                    viewHeartBeat = isChecked;
                    break;
                case 1:
                    break;
            }
        }

        private async void connectButton_Click(object sender, EventArgs e)
        {
            if (connectButton.Text.Equals("Connect", StringComparison.OrdinalIgnoreCase))
            {
                connectServer();
                connectButton.Text = "Disconnect";
                connectButton.BackColor = Color.OrangeRed;
            }
            else
            {
                webSocketClient.Disconnect();
                connectButton.Text = "Connect";
                connectButton.BackColor = Color.Aquamarine;
            }
        }
    }
}
