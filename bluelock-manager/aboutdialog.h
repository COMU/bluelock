#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtWidgets/QDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QLabel>
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
    void openLicense(bool state);
    void openCredits(bool state);

private:
    Ui::AboutDialog *ui;

    QTextBrowser *contributorBrowser;
    QTextBrowser *licenseBrowser;
};

#endif // ABOUTDIALOG_H
