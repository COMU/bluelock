#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(released()),
            this, SLOT(closeWindow()));
    connect(ui->pushButtonLicense, SIGNAL(toggled(bool)),
            this, SLOT(openLicense(bool)));
    connect(ui->pushButtonCredits, SIGNAL(toggled(bool)),
            this, SLOT(openCredits(bool)));

    ui->labelApplicationName->setText(APPLICATION_DISPLAY_NAME);
    ui->labelApplicationVersion->setText(APPLICATION_VERSION);
    ui->labelApplicationDescription->setText(tr(
    "Configure your trusted bluetooth devices\n"
    "which are used during login via pam_bluelock"));
    ui->labelCopyright->setText(
    "Copyright © 2017 – Çağrı Ulaş\n"
    "Copyright © 2017 – Canberk Koç");
    ui->labelLicense->setText(tr(
    "This program comes with absolutely no warranty.\n"
    "See the GNU General Public License, version 3 or "
    "later for details."));

    QFont serifFont("", 10, QFont::Normal);

    contributorBrowser = new QTextBrowser(ui->verticalWidget);
    contributorBrowser->resize(ui->verticalWidget->sizeHint());
    contributorBrowser->setHidden(true);
    contributorBrowser->setHtml(tr(
    "<center><table>"
    "<tr>"
    "<th>Created By "
    "<th align=\"left\">Çağrı Ulaş"
    "<tr>"
    "<th>"
    "<th align=\"left\">Canberk Koç"
    "</table></center>"));
    contributorBrowser->setFont(serifFont);

    licenseBrowser = new QTextBrowser(ui->verticalWidget);
    licenseBrowser->resize(ui->verticalWidget->sizeHint());
    licenseBrowser->setHidden(true);
    licenseBrowser->setText(tr(
    "This program is free software: you can redistribute it and/or modify "
    "it under the terms of the GNU General Public License as published by "
    "the Free Software Foundation, either version 3 of the License, or "
    "(at your option) any later version."
    "\n\n"
    "This program is distributed in the hope that it will be useful,"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
    "GNU General Public License for more details."
    "\n\n"
    "You should have received a copy of the GNU General Public License "
    "along with this program.  If not, see <http://www.gnu.org/licenses/>."));
    licenseBrowser->setFont(serifFont);

    this->setFixedSize(this->sizeHint());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::closeWindow()
{
    ui->pushButtonCredits->setChecked(false);
    ui->pushButtonLicense->setChecked(false);

    hide();
}

void AboutDialog::openLicense(bool state)
{
    if (!state)
        licenseBrowser->setHidden(true);
    else {
        contributorBrowser->setHidden(true);
        licenseBrowser->setVisible(true);
        ui->pushButtonCredits->setChecked(false);
    }
}

void AboutDialog::openCredits(bool state)
{
    if (!state)
        contributorBrowser->setHidden(true);
    else {
        contributorBrowser->setVisible(true);
        licenseBrowser->setHidden(true);
        ui->pushButtonLicense->setChecked(false);
    }
}
