#ifndef DIFFWINDOW_H
#define DIFFWINDOW_H

#include <KXmlGuiWindow>
#include <git/gitfile.h>
#include <QObject>

class DiffTreeModel;
class DiffWidget;
class DiffTreeView;
class FilesModel;
class DiffWindow : public KXmlGuiWindow
{
    Q_OBJECT

    Git::File _oldFile;
    Git::File _newFile;

    QString _oldBranch;
    QString _newBranch;

    FilesModel *_filesModel;
    DiffTreeModel *_diffModel;
    DiffWidget *_diffWidget;
    DiffTreeView *_treeView;

    void initActions();
    void init();

public:
    explicit DiffWindow();
    DiffWindow(const Git::File &oldFile, const Git::File &newFile);
    DiffWindow(const QString &oldBranch, const QString &newBranch);

private slots:
    void fileOpen();
    void on_treeView_fileSelected(const QString &file);
};

#endif // DIFFWINDOW_H
