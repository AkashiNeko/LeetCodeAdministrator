#include "widget.h"
#include "ui_widget.h"
#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

Widget::Widget(QWidget *parent) :QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    path = QDir::currentPath();
    this->md = new MarkDown((path + "/README.md").toLocal8Bit().data());
    init();
}

Widget::~Widget() {
    delete md;
    delete ui;
}

void Widget::init() {
    if(!md->read()) {
        QMessageBox::critical(this, "错误",
            "当前目录：" + path + \
            "\n\n这不是Akashi的LeetCode工作目录！" \
            "\n\n请将该程序移动至包含 README.md 的相同目录下！"
        );
        exit(0);
    }
    setNumbers();

    ui->ledCurDir->setText(path);

    QDate curDate = QDate::currentDate();
    date = QString::number(curDate.year()) + '.' + QString::number(curDate.month());
    ui->ledInfoDate->setText(date);
    QDir dir(path + "/" + date);
    if (!dir.exists()) {
        bool ismkdir = dir.mkdir(path + "/" + date);
        if(!ismkdir) {
            QMessageBox::critical(this, "错误", "创建文件夹 " + date + " 失败。");
            exit(0);
        }
    }
    showMD();
}

void Widget::showMD() {
    QString tedMdText;
    int sz = md->size() >= 10 ? 10 : md->size();
    for (int i = 0; i < sz; ++i) {
        tedMdText += QString().fromStdString(md->at(i));
        tedMdText += '\n';
    }
    ui->tedMd->setText(tedMdText);
}

void Widget::setNumbers() {
    ui->ledMdTotal->setText(QString::number(md->total));
    ui->ledMdEasy->setText(QString::number(md->easy));
    ui->ledMdMedium->setText(QString::number(md->medium));
    ui->ledMdHard->setText(QString::number(md->hard));
}

bool Widget::writeCode() {
    string code = ui->tedCode->toPlainText().toStdString();
    assert(!code.empty());
    QString filePath = path + "/" + date + "/" + ui->ledInfoZhName->text() + \
        "（" + ui->ledInfoEnName->text() + "）" + getExtern();
    QByteArray bytes = filePath.toLocal8Bit();
    fstream file;
    file.open(bytes.data(), ios::out);
    if (!file.is_open())
        return false;
    file << "// " << ui->ledInputLink->text().toStdString() << endl;
    file << "// " << date.toLocal8Bit().data() << endl << endl;
    file << code << endl;
    file.close();
    return true;
}

const char *Widget::getExtern()
{
    static const char* ext[] = {
        ".cpp",
        ".c",
        ".java",
        ".py"
    };
    return ext[ui->cboType->currentIndex()];
}

const char *Widget::getDiff() {
    static const char* diff[] = {
        "💚",
        "🧡",
        "❤"
    };
    return diff[ui->cboDiff->currentIndex()];
}

void Widget::on_ledInputName_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        ui->ledInfoNum->setText("");
        ui->ledInfoZhName->setText("");
    }
    else if (text[0].isDigit()) {
        int i = 0;
        while (i < text.size()) {
            if (!text[i].isDigit()) {
                ui->ledInfoNum->setText(text.left(i));
                break;
            }
            ++i;
        }
        while (i < text.size()) {
            if (text[i] != '.' && text[i] != ' ') {
                ui->ledInfoZhName->setText(text.mid(i));
                break;
            }
            ++i;
        }
    }
    else {
        ui->ledInfoNum->setText("?");
        ui->ledInfoZhName->setText(text);
    }
}

void Widget::on_ledInputLink_textChanged(const QString &text) {
    static const int start = 29;
    if (text.size() > start) {
        int i = start;
        while (i < text.size()) {
            if (text[i] == '/')
                break;
            ++i;
        }
        ui->ledInfoEnName->setText(text.mid(start, i - start));
    }
    else {
        ui->ledInfoEnName->setText("");
    }
}

void Widget::on_btnOpenFolder_clicked() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Widget::on_btnTidy_clicked() {
    md->tidy();
    showMD();
    setNumbers();
    md->setDiffNum();
    if (!md->save())
        QMessageBox::critical(this, "错误", "写入文件 README.md 失败");
    QMessageBox::about(this, "提示", "整理完成");
}

void Widget::on_btnAdd_clicked() {
    if (ui->ledInfoNum->text().isEmpty() || ui->ledInfoZhName->text().isEmpty() || ui->ledInfoEnName->text().isEmpty()) {
        QMessageBox::critical(this, "错误", "题目信息未填写完整");
        return;
    }
    switch (ui->cboDiff->currentIndex()) {
    case Easy:
        ++md->easy;
        break;
    case Medium:
        ++md->medium;
        break;
    case Hard:
        ++md->hard;
        break;
    }
    ++md->total;
    setNumbers();
    md->setDiffNum();
    QString markdown = "|" + QString::number(md->total) + "|" + ui->ledInfoNum->text() + "|" +
        ui->ledInfoZhName->text() + "|[LeetCode](" + ui->ledInputLink->text() + ")|[题解](" +
        date + "/" + QUrl::toPercentEncoding(ui->ledInfoZhName->text() + "（" +
        ui->ledInfoEnName->text() + "）" + getExtern()).data() + ")|" + getDiff() + "|" +
        date + "." + QString::number(QDate::currentDate().day()) + "|";
    md->add(markdown.toStdString());
    this->showMD();
}

void Widget::on_btnSave_clicked() {
    assert(!md->empty());
    if (ui->tedCode->toPlainText().isEmpty())
        QMessageBox::critical(this, "错误", "请输入代码");
    else if (!writeCode())
        QMessageBox::critical(this, "错误", "写入代码文件失败");
    else if (!md->save())
        QMessageBox::critical(this, "错误", "写入文件 README.md 失败");
    else
        QMessageBox::about(this, "提示", "保存成功");
}
