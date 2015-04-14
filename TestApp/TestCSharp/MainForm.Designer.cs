namespace TestCSharp
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
            this.comboTestList = new System.Windows.Forms.ComboBox();
            this.BtnRunTest = new System.Windows.Forms.Button();
            this.textOutput = new System.Windows.Forms.TextBox();
            this.textInput = new System.Windows.Forms.TextBox();
            this.textID = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // comboTestList
            // 
            this.comboTestList.FormattingEnabled = true;
            this.comboTestList.Location = new System.Drawing.Point(12, 12);
            this.comboTestList.Name = "comboTestList";
            this.comboTestList.Size = new System.Drawing.Size(343, 21);
            this.comboTestList.TabIndex = 0;
            // 
            // BtnRunTest
            // 
            this.BtnRunTest.Location = new System.Drawing.Point(361, 10);
            this.BtnRunTest.Name = "BtnRunTest";
            this.BtnRunTest.Size = new System.Drawing.Size(75, 23);
            this.BtnRunTest.TabIndex = 1;
            this.BtnRunTest.Text = "Run Test";
            this.BtnRunTest.UseVisualStyleBackColor = true;
            this.BtnRunTest.Click += new System.EventHandler(this.OnRunTestClicked);
            // 
            // textOutput
            // 
            this.textOutput.Location = new System.Drawing.Point(13, 40);
            this.textOutput.MaxLength = 16000;
            this.textOutput.Multiline = true;
            this.textOutput.Name = "textOutput";
            this.textOutput.ReadOnly = true;
            this.textOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textOutput.Size = new System.Drawing.Size(628, 311);
            this.textOutput.TabIndex = 2;
            this.textOutput.TabStop = false;
            // 
            // textInput
            // 
            this.textInput.AcceptsReturn = true;
            this.textInput.Location = new System.Drawing.Point(12, 357);
            this.textInput.Name = "textInput";
            this.textInput.Size = new System.Drawing.Size(628, 20);
            this.textInput.TabIndex = 3;
            this.textInput.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
            // 
            // textID
            // 
            this.textID.Location = new System.Drawing.Point(453, 12);
            this.textID.Name = "textID";
            this.textID.Size = new System.Drawing.Size(188, 20);
            this.textID.TabIndex = 4;
            this.textID.Text = "나몰라1";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(653, 389);
            this.Controls.Add(this.textID);
            this.Controls.Add(this.textInput);
            this.Controls.Add(this.textOutput);
            this.Controls.Add(this.BtnRunTest);
            this.Controls.Add(this.comboTestList);
            this.Name = "MainForm";
            this.Text = "BR Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboTestList;
        private System.Windows.Forms.Button BtnRunTest;
        private System.Windows.Forms.TextBox textOutput;
        private System.Windows.Forms.TextBox textInput;
        private System.Windows.Forms.TextBox textID;
    }
}

