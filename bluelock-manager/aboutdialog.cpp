#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(released()),
            this, SLOT(closeWindow()));
    connect(ui->pushButtonLicense, SIGNAL(released()),
            this, SLOT(openLicense()));
    connect(ui->pushButtonCredits, SIGNAL(released()),
            this, SLOT(openCredits()));
    ui->labelApplicationName->setText(APPLICATION_DISPLAY_NAME);
    ui->labelApplicationVersion->setText(APPLICATION_VERSION);
    ui->labelApplicationDescription->setText(
                tr("Configure your trusted bluetooth devices\n"
                   "which are used during login via pam_bluelock"));
    ui->labelCopyright->setText("Copyright © 2017 – Çağrı Ulaş\n"
                                "Copyright © 2017 – Canberk Koç");
    ui->labelLicense->setText(
                tr("This program comes with absolutely no warranty.\n"
                   "See the GNU General Public License, version 3 or "
                   "later for details."));

    contributorBrowser = new QTextBrowser(ui->verticalWidget);
    contributorBrowser->resize(ui->verticalWidget->sizeHint());
    contributorBrowser->setHidden(true);

    this->setFixedSize(this->sizeHint());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::closeWindow()
{
    hide();
}

void AboutDialog::openLicense()
{
    QDesktopServices::openUrl(QUrl("https://www.gnu.org/licenses/gpl-3.0.html"));
}

void AboutDialog::openCredits()
{
    if (contributorBrowser->isVisible())
        contributorBrowser->setHidden(true);
    else
        contributorBrowser->setVisible(true);
}
