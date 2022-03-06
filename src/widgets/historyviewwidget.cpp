#include "historyviewwidget.h"
#include "diffwindow.h"
#include "models/historymodel.h"
#include "dialogs/filestreedialog.h"
#include "git/gitlog.h"
#include <QDebug>
#include <QMenu>
#include "widgets/graphpainter.h"
#include "actions/commitactions.h"

HistoryViewWidget::HistoryViewWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _historyModel = new HistoryModel(this);
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);

    _actions = new CommitActions(Git::Manager::instance(), this);
}

HistoryViewWidget::HistoryViewWidget(Git::Manager *git, GitKlientWindow *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
    _historyModel = new HistoryModel(this);
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);

    _actions = new CommitActions(git, this);
}

void HistoryViewWidget::setBranch(const QString &branchName)
{
    treeViewHistory->setItemDelegateForColumn(0, nullptr);
    _historyModel->setBranch(branchName);
    if (_historyModel->rowCount(QModelIndex()))
        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

void HistoryViewWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
}

void HistoryViewWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
}

void HistoryViewWidget::on_treeViewHistory_itemActivated(const QModelIndex &index)
{
    auto log = _historyModel->log(index);
    if (!log)
        return;

    textBrowser->setLog(log);
}

void HistoryViewWidget::on_textBrowser_hashClicked(const QString &hash)
{
    auto index = _historyModel->findIndexByHash(hash);
    if (index.isValid()) {
        treeViewHistory->setCurrentIndex(index);
        on_treeViewHistory_itemActivated(index);
    }
}

void HistoryViewWidget::on_textBrowser_fileClicked(const QString &file)
{
    auto log = textBrowser->log();

    Git::File oldFile;
    Git::File newFile(log->commitHash(), file);
    if (log->parents().size()) {
        oldFile = {log->parents().first(), file};
    }
    auto diffWin = new DiffWindow(oldFile, newFile);
    diffWin->setWindowModality(Qt::ApplicationModal);
    diffWin->setAttribute(Qt::WA_DeleteOnClose, true);
    diffWin->show();
}

void HistoryViewWidget::on_treeViewHistory_customContextMenuRequested(const QPoint &pos)
{
    auto log = _historyModel->log(treeViewHistory->currentIndex());
    if (!log)
        return;
    _actions->setCommitHash(log->commitHash());

    _actions->popup(treeViewHistory->mapToGlobal(pos));
}

void HistoryViewWidget::git_pathChanged()
{
    _historyModel->reload();
    if (_historyModel->rowCount(QModelIndex()))
        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

