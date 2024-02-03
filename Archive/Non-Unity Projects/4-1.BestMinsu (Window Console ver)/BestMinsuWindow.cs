using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BestMinsu_WindowApp {
    public partial class BestMinsuWindow : Form {
        public BestMinsuWindow() {
            InitializeComponent();
            Director.Initialize(richTextBox1);
        }

		private void radioButton1_CheckedChanged(object sender, EventArgs e) {
			Director.CurrentRunMode = Director.RunMode.AnalysisFromWeb;
		}

		private void radioButton2_CheckedChanged(object sender, EventArgs e) {
			Director.CurrentRunMode = Director.RunMode.AnalysisFromJson;
		}
		
		private void button1_Click(object sender, EventArgs e) {
			button1.Enabled = false;
			button2.Enabled = true;
			Director.Run(() => {
				button1.Invoke(new MethodInvoker(() => button1.Enabled = true));
				button2.Invoke(new MethodInvoker(() => button2.Enabled = false));
			});
		}

		private void button2_Click(object sender, EventArgs e) {
			var ret = MessageBox.Show("분석을 취소합니다. 현재까지의 분석결과는 저장되지만 나중에 재시도할 때는 처음부터 다시 분석해야 합니다.", "ㄹㅇ?", MessageBoxButtons.OKCancel);

			if (ret == DialogResult.OK) {
				Director.Stop();
			}
		}
    }
}