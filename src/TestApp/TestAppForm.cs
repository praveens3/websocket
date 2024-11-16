using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace TestApp
{
    public partial class TestAppForm : Form
    {
        private WebSocketClient webSocketClient;
        private CHttpClient httpClient;
        private bool viewHeartBeat = false;
        public TestAppForm()
        {
            InitializeComponent();
            webSocketClient = new WebSocketClient(UpdateViewerBox);
            httpClient = new CHttpClient();
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

        private void fileUploadButton_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Length == 0 && !textBox1.Text.Contains("."))
                return;

            try
            {
                var progress = new Progress<int>(percent =>
                {
                    statusLabel.Text = $"Upload progress: {percent}%";
                    //progressBar1.Value = percent; // Assuming you have a ProgressBar named progressBar
                });

                _=httpClient.UploadFileAsync(textBox1.Text, "https://localhost:8080");

                statusLabel.Text = "File upload completed successfully.";
            }
            catch (Exception ex)
            {
                statusLabel.Text = $"Error uploading file to server: {ex.Message}";
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        //    if (textBox1.Text.Length == 0)
        //    {
        //        textBox1.Text = "double click to select file/type the file path";
        //        textBox1.ForeColor = Color.LightGray;
        //    }
        }

        private void textBox1_Enter(object sender, EventArgs e)
        {
            // Clear placeholder text when the TextBox gains focus
            if (textBox1.Text == "double click to select file/type the file path")
            {
                textBox1.Text = "";
                textBox1.ForeColor = Color.Black;
            }
        }

        private void textBox1_Leave(object sender, EventArgs e)
        {
            // Restore placeholder text if the TextBox is empty when it loses focus
            if (string.IsNullOrWhiteSpace(textBox1.Text))
            {
                textBox1.Text = "double click to select file/type the file path";
                textBox1.ForeColor = Color.LightGray;
            }
        }
        private void textBox1_DoubleClick(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "All Files (*.*)|*.*";
            dialog.Title = "Select a File to upload";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = dialog.FileName;
                textBox1.ForeColor = Color.Black;
            }
        }
    }
}
