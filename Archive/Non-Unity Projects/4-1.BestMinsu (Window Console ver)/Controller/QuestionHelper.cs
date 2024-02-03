using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

public class QuestionHelper {
    class Content {
        public string Text { get; }
        public bool Answer { get; }

        public Content(string text, bool answer) {
            Text = text;
            Answer = answer;
        }
    }

    private static IReadOnlyList<Content> Contents { get; } = new List<Content>() {
        new Content("Q. 매수한 주식의 수익률이 -60% 만큼 손실이 날 경우, 성급한 매수를 자제하고 손절을 고려해야 한다.", false),
        new Content("Q. 매수한 주식이 급락하면 추가 매수의 기회이므로 오히려 좋다.", true),
        new Content("Q. PER 100배인 기업이 50이 될 경우 저평가된 종목이니 매수하는 것이 좋다.", false),
        new Content("Q. 당장은 적자인 기업이라도 미래 성장성이 주목할 만하면 분할 매수한다.", false),
        new Content("Q. 시장에 과한 버블이 발생했다면 숏 포지션의 ETF를 고려해 본다.", false),
        new Content("Q. 주식 초보라면 절반 이상을 ETF에 투자하는 것이 좋다.", true),
        new Content("Q. 433 전법이란 자본금의 40%는 안정적인 주식, 30%는 미래에 떠오를 저평가 종목, 30%는 현재 대세인 성장주로 나누어 운용하는 것이다.", false),
    };

    public static bool CheckQuestion() {
        var random = new Random();
        var contents = Contents.OrderBy(_ => random.Next()).Take(2).ToList();
        if (!contents.Any()) return true;
        
        string title = "최고민수 OX 퀴즈";
        MessageBoxButtons buttons = MessageBoxButtons.YesNo;
        MessageBoxIcon icon = MessageBoxIcon.Question;

        foreach (var content in contents) {
            string message = content.Text;
            DialogResult result = MessageBox.Show(message, title, buttons);
            if (result != DialogResult.Yes && result != DialogResult.No) return false;

            bool userAnswer = result == DialogResult.Yes;
            var isCorrect = userAnswer == content.Answer;
            
            if (!isCorrect) {
                MessageBox.Show("최고민수 퀴즈를 맞추지 못했습니다.");
                AnalysisHelper.ShowMessage("최고민수 퀴즈를 통과해야 분석을 시작할 수 있습니다.");
                return false;
            }
        }

        return true;
    }
}