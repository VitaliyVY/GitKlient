#include "fetchdialog.h"

#include "runnerdialog.h"
#include "git/commandfetch.h"
#include "git/gitmanager.h"

FetchDialog::FetchDialog(Git::Manager *git, QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    auto g = git;
    if(!g)
        g = Git::Manager::instance();
    comboBoxRemote->addItems(g->remotes());
    comboBoxBranch->addItems(g->branches());

    comboBoxRemote->setCurrentText(g->currentBranch());

}

void FetchDialog::on_buttonBox_accepted()
{
    Git::CommandFetch cmd;

    cmd.setRemote(comboBoxRemote->currentText());
    cmd.setBranch(comboBoxBranch->currentText());
    cmd.setSquash(checkBoxSquash->isChecked());
    cmd.setNoFf(checkBoxNoFastForward->isChecked());
    cmd.setFfOnly(checkBoxFastForwardOnly->isChecked());
    cmd.setNoCommit(checkBoxNoCommit->isChecked());
    cmd.setPrune(checkBoxPrune->isChecked());
    cmd.setTags(checkBoxTags->isChecked());

    RunnerDialog d(this);
    d.run(&cmd);
    d.exec();

    accept();
}
