#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "markdown.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

namespace DiffColor {
    static const QString EASY = "(0, 170, 0)";
    static const QString MEDIUM = "(240, 160, 0)";
    static const QString HARD = "(220, 0, 0)";
    static const QString TOTAL = "(0, 85, 255)";
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void init();
    void showMD();
    void setNumbers();
    bool writeCode();
    const char* getExtern();
    const char* getDiff();

private slots:
    void on_ledInputName_textChanged(const QString &text);
    void on_ledInputLink_textChanged(const QString &text);
    void on_btnOpenFolder_clicked();
    void on_btnTidy_clicked();
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_cboDiff_currentIndexChanged(int index);

private:
    QString path;
    QString date;
    Ui::Widget *ui;
    MarkDown* md;
};
#endif // WIDGET_H
