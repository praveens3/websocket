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
        private string URI = "://localhost:7002";
        public TestAppForm()
        {
            InitializeComponent();
            webSocketClient = new WebSocketClient(UpdateViewerBox, UpdateLogViewer);
            httpClient = new CHttpClient(UpdateLogViewer);
        }

        private async void connectServer()
        {
            try
            {
                Uri serverUri = new Uri("ws" + URI);
                this.Text = this.Text + " " + serverUri.ToString();
                await webSocketClient.ConnectAsync(serverUri);
                statusLabel.Text = "Connected to WebSocket server!";
                UpdateLogViewer("Connected to WebSocket server!");
            }
            catch (Exception ex)
            {
                statusLabel.Text = $"Error connecting to server: " + ex.Message;
                connectButton.Text = "Connect";
                connectButton.BackColor = Color.Aquamarine;
                UpdateLogViewer($"Error connecting to server: " + ex.Message);
                UpdateLogViewer($"Error connecting to server: " + ex.InnerException);

            }
        }
        private void TestAppUI_FormClosing(object sender, FormClosingEventArgs e)
        {
            webSocketClient.Disconnect();
            statusLabel.Text = "Server disconnected";
            UpdateLogViewer("Server disconnected");
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
            viewerBox.ScrollToCaret();
        }

        private async void sendButton_Click(object sender, EventArgs e)
        {
            await webSocketClient.SendMessageAsync(editorBox.Text);
            UpdateLogViewer("Message sent: " + editorBox.Text);
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

        private async void fileUploadButton_Click(object sender, EventArgs e)
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
                UpdateLogViewer("start uploading file: " + textBox1.Text);
                await httpClient.UploadFileAsync(textBox1.Text, "http" + URI);
            }
            catch (Exception ex)
            {
                UpdateLogViewer($"Error uploading file to server: {ex.Message}");
            }
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

        private void LogButton_Click(object sender, EventArgs e)
        {
            if (LogButton.Text == ">")
            {
                this.Width += 400;
                LogButton.Text = "<";
            }
            else
            {
                this.Width = 824;
                LogButton.Text = ">";
            }
        }

        // Method to safely update the RichTextBox from a background thread
        private void UpdateLogViewer(string message)
        {
            if (LogViewer.InvokeRequired)
            {
                LogViewer.Invoke((MethodInvoker)delegate
                {
                    LogViewer.AppendText(message + Environment.NewLine);
                });
            }
            else
            {
                LogViewer.AppendText(message + Environment.NewLine);
            }
            LogViewer.ScrollToCaret();
        }

    }
}