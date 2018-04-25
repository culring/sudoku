#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QStyledItemDelegate>

class CustomDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CustomDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // CUSTOMDELEGATE_H
