namespace BR.FTPSync
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxLocalPath = new System.Windows.Forms.TextBox();
            this.textBoxRemotePath = new System.Windows.Forms.TextBox();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Local:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 35);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Remote: ";
            // 
            // textBoxLocalPath
            // 
            this.textBoxLocalPath.Location = new System.Drawing.Point(70, 11);
            this.textBoxLocalPath.Name = "textBoxLocalPath";
            this.textBoxLocalPath.ReadOnly = true;
            this.textBoxLocalPath.Size = new System.Drawing.Size(564, 20);
            this.textBoxLocalPath.TabIndex = 2;
            // 
            // textBoxRemotePath
            // 
            this.textBoxRemotePath.Location = new System.Drawing.Point(69, 35);
            this.textBoxRemotePath.Name = "textBoxRemotePath";
            this.textBoxRemotePath.ReadOnly = true;
            this.textBoxRemotePath.Size = new System.Drawing.Size(564, 20);
            this.textBoxRemotePath.TabIndex = 2;
            // 
            // textBoxLog
            // 
            this.textBoxLog.Location = new System.Drawing.Point(16, 61);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ReadOnly = true;
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxLog.Size = new System.Drawing.Size(617, 289);
            this.textBoxLog.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(646, 362);
            this.Controls.Add(this.textBoxLog);
            this.Controls.Add(this.textBoxRemotePath);
            this.Controls.Add(this.textBoxLocalPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "MainForm";
            this.Text = "Synchronize FTP";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxLocalPath;
        private System.Windows.Forms.TextBox textBoxRemotePath;
        private System.Windows.Forms.TextBox textBoxLog;
    }
}

