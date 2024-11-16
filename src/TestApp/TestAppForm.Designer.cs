namespace TestApp
{
    partial class TestAppForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            viewerBox = new RichTextBox();
            editorBox = new RichTextBox();
            sendButton = new Button();
            panel1 = new Panel();
            checkedListBox1 = new CheckedListBox();
            connectButton = new Button();
            fileUploadButton = new Button();
            statusLabel = new Label();
            textBox1 = new TextBox();
            progressBar1 = new ProgressBar();
            panel1.SuspendLayout();
            SuspendLayout();
            // 
            // viewerBox
            // 
            viewerBox.Location = new Point(191, 34);
            viewerBox.Name = "viewerBox";
            viewerBox.Size = new Size(597, 248);
            viewerBox.TabIndex = 0;
            viewerBox.Text = "";
            // 
            // editorBox
            // 
            editorBox.Location = new Point(191, 367);
            editorBox.Name = "editorBox";
            editorBox.Size = new Size(523, 67);
            editorBox.TabIndex = 1;
            editorBox.Text = "";
            // 
            // sendButton
            // 
            sendButton.Location = new Point(720, 349);
            sendButton.Name = "sendButton";
            sendButton.Size = new Size(68, 86);
            sendButton.TabIndex = 2;
            sendButton.Text = "Send";
            sendButton.UseVisualStyleBackColor = true;
            sendButton.Click += sendButton_Click;
            // 
            // panel1
            // 
            panel1.Controls.Add(checkedListBox1);
            panel1.Controls.Add(connectButton);
            panel1.Location = new Point(8, 23);
            panel1.Name = "panel1";
            panel1.Size = new Size(159, 415);
            panel1.TabIndex = 3;
            // 
            // checkedListBox1
            // 
            checkedListBox1.FormattingEnabled = true;
            checkedListBox1.Items.AddRange(new object[] { "View Heartbeat", "enable s3" });
            checkedListBox1.Location = new Point(6, 10);
            checkedListBox1.Name = "checkedListBox1";
            checkedListBox1.Size = new Size(120, 94);
            checkedListBox1.TabIndex = 1;
            checkedListBox1.ItemCheck += checkedListBox1_ItemCheck;
            // 
            // connectButton
            // 
            connectButton.Location = new Point(8, 362);
            connectButton.Name = "connectButton";
            connectButton.Size = new Size(148, 41);
            connectButton.TabIndex = 0;
            connectButton.Text = "Connect";
            connectButton.UseVisualStyleBackColor = true;
            connectButton.Click += connectButton_Click;
            // 
            // fileUploadButton
            // 
            fileUploadButton.Location = new Point(720, 302);
            fileUploadButton.Name = "fileUploadButton";
            fileUploadButton.Size = new Size(68, 23);
            fileUploadButton.TabIndex = 2;
            fileUploadButton.Text = "Upload";
            fileUploadButton.UseVisualStyleBackColor = true;
            fileUploadButton.Click += fileUploadButton_Click;
            // 
            // statusLabel
            // 
            statusLabel.AutoSize = true;
            statusLabel.Location = new Point(191, 285);
            statusLabel.Name = "statusLabel";
            statusLabel.Size = new Size(86, 15);
            statusLabel.TabIndex = 4;
            statusLabel.Text = "Connect server";
            statusLabel.TextAlign = ContentAlignment.BottomRight;
            // 
            // textBox1
            // 
            textBox1.ForeColor = Color.LightGray;
            textBox1.Location = new Point(192, 303);
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(522, 23);
            textBox1.TabIndex = 5;
            textBox1.Text = "double click to select file/type the file path";
            textBox1.TextChanged += textBox1_TextChanged;
            textBox1.DoubleClick += textBox1_DoubleClick;
            textBox1.Enter += textBox1_Enter;
            textBox1.Leave += textBox1_Leave;
            // 
            // progressBar1
            // 
            progressBar1.Location = new Point(236, 332);
            progressBar1.Name = "progressBar1";
            progressBar1.Size = new Size(411, 23);
            progressBar1.TabIndex = 6;
            // 
            // TestAppForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(progressBar1);
            Controls.Add(fileUploadButton);
            Controls.Add(textBox1);
            Controls.Add(statusLabel);
            Controls.Add(panel1);
            Controls.Add(sendButton);
            Controls.Add(editorBox);
            Controls.Add(viewerBox);
            Name = "TestAppForm";
            Text = "TestApp";
            panel1.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private RichTextBox viewerBox;
        private RichTextBox editorBox;
        private Button sendButton;
        private Panel panel1;
        private Button connectButton;
        private Label statusLabel;
        private CheckedListBox checkedListBox1;
        private Button fileUploadButton;
        private TextBox textBox1;
        private ProgressBar progressBar1;
    }
}
