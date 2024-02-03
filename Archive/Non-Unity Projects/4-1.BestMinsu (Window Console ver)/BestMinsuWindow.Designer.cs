namespace BestMinsu_WindowApp {
    partial class BestMinsuWindow {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }

            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
	        this.richTextBox1 = new System.Windows.Forms.RichTextBox();
	        this.button1 = new System.Windows.Forms.Button();
	        this.radioButton1 = new System.Windows.Forms.RadioButton();
	        this.radioButton2 = new System.Windows.Forms.RadioButton();
	        this.button2 = new System.Windows.Forms.Button();
	        this.SuspendLayout();
	        // 
	        // richTextBox1
	        // 
	        this.richTextBox1.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (129)));
	        this.richTextBox1.Location = new System.Drawing.Point(35, 50);
	        this.richTextBox1.Name = "richTextBox1";
	        this.richTextBox1.ReadOnly = true;
	        this.richTextBox1.Size = new System.Drawing.Size(740, 596);
	        this.richTextBox1.TabIndex = 0;
	        this.richTextBox1.Text = "";
	        // 
	        // button1
	        // 
	        this.button1.Font = new System.Drawing.Font("메이플스토리", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (129)));
	        this.button1.Location = new System.Drawing.Point(818, 517);
	        this.button1.Name = "button1";
	        this.button1.Size = new System.Drawing.Size(185, 53);
	        this.button1.TabIndex = 1;
	        this.button1.Text = "분석 시작";
	        this.button1.UseVisualStyleBackColor = true;
	        this.button1.Click += new System.EventHandler(this.button1_Click);
	        // 
	        // radioButton1
	        // 
	        this.radioButton1.AutoSize = true;
	        this.radioButton1.Checked = true;
	        this.radioButton1.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (129)));
	        this.radioButton1.Location = new System.Drawing.Point(818, 60);
	        this.radioButton1.Name = "radioButton1";
	        this.radioButton1.Size = new System.Drawing.Size(174, 36);
	        this.radioButton1.TabIndex = 2;
	        this.radioButton1.TabStop = true;
	        this.radioButton1.Text = "웹에서 탐색";
	        this.radioButton1.UseVisualStyleBackColor = true;
	        this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
	        // 
	        // radioButton2
	        // 
	        this.radioButton2.AutoSize = true;
	        this.radioButton2.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (129)));
	        this.radioButton2.Location = new System.Drawing.Point(818, 105);
	        this.radioButton2.Name = "radioButton2";
	        this.radioButton2.Size = new System.Drawing.Size(253, 36);
	        this.radioButton2.TabIndex = 3;
	        this.radioButton2.Text = "Json 파일에서 탐색";
	        this.radioButton2.UseVisualStyleBackColor = true;
	        this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
	        // 
	        // button2
	        // 
	        this.button2.Enabled = false;
	        this.button2.Font = new System.Drawing.Font("메이플스토리", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (129)));
	        this.button2.Location = new System.Drawing.Point(818, 593);
	        this.button2.Name = "button2";
	        this.button2.Size = new System.Drawing.Size(185, 53);
	        this.button2.TabIndex = 4;
	        this.button2.Text = "분석 중단";
	        this.button2.UseVisualStyleBackColor = true;
	        this.button2.Click += new System.EventHandler(this.button2_Click);
	        // 
	        // BestMinsuWindow
	        // 
	        this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
	        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
	        this.ClientSize = new System.Drawing.Size(1112, 720);
	        this.Controls.Add(this.button2);
	        this.Controls.Add(this.radioButton2);
	        this.Controls.Add(this.radioButton1);
	        this.Controls.Add(this.button1);
	        this.Controls.Add(this.richTextBox1);
	        this.Name = "BestMinsuWindow";
	        this.Text = "BestMinsu";
	        this.ResumeLayout(false);
	        this.PerformLayout();
        }

        private System.Windows.Forms.Button button2;

        private System.Windows.Forms.RichTextBox richTextBox1;

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.RadioButton radioButton2;
	}
}