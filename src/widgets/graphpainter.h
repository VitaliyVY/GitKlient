#ifndef GRAPHPAINTER_H
#define GRAPHPAINTER_H

#include <QStyledItemDelegate>
#include "git/gitgraphlane.h"

namespace Git {
class LogsCache;
};
class GraphPainter : public QStyledItemDelegate
{
    Q_OBJECT
    Git::LogsCache *_model;
    QVector<QColor> _colors;

public:
    GraphPainter(Git::LogsCache *model, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paintLane(QPainter *painter, const Git::GraphLane &lane, int index) const;
    int colX(const int &col) const;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // GRAPHPAINTER_H
