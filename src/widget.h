#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "markdown.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

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

private:
    QString path;
    QString date;
    Ui::Widget *ui;
    MarkDown* md;
};
#endif // WIDGET_H
