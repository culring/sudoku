#include "customdelegate.h"
#include <QPainter>
#include "gameboardmodel.h"

CustomDelegate::CustomDelegate()
{

}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    int row = index.row();
    int column = index.column();
    int squareRow = row/3;
    int squareColumn = column/3;
    int squareOrder = 3*squareRow + squareColumn;

    if(squareOrder%2){
        painter->fillRect(option.rect, QColor(47, 79, 79));
    }
    else{
        painter->fillRect(option.rect, QColor(43, 43, 43));
    }

    QStyleOptionViewItem op(option);
    op.displayAlignment = Qt::AlignCenter;
    op.font = QFont("Arial", 20);

    auto &model = *qobject_cast<const GameBoardModel*>(index.model());
    if(model.isValueDuplicate(index)){
        if(model.isLevelField(index)){
            op.palette.setBrush(QPalette::Text, Qt::darkRed);
            op.palette.setBrush(QPalette::HighlightedText, Qt::darkRed);
        }
        else{
            op.palette.setBrush(QPalette::Text, Qt::red);
            op.palette.setBrush(QPalette::HighlightedText, Qt::red);
        }
    }
    else{
        if(model.isLevelField(index)){
            op.palette.setBrush(QPalette::Text, Qt::gray);
            op.palette.setBrush(QPalette::HighlightedText, Qt::gray);
        }
        else{
            op.palette.setBrush(QPalette::Text, Qt::white);
        }
    }

    QStyledItemDelegate::paint(painter, op, index);
}
