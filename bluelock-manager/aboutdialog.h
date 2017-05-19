#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtWidgets/QDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QtWidgets/QTextBrowser>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void closeWindow();
    void openLicense();
    void openCredits();

private:
    Ui::AboutDialog *ui;

    QTextBrowser *contributorBrowser;
};

#endif // ABOUTDIALOG_H
