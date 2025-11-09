#include "list_item_base.h"

#include <QPainter>
#include <QStyleOption>

ListItemBase::ListItemBase(QWidget *parent) : QWidget(parent) {}

void ListItemBase::SetItemType(ListItemType itemType) { item_type_ = itemType; }

ListItemType ListItemBase::GetItemType() { return item_type_; }

// 绘制列表项
// void ListItemBase::paintEvent(QPaintEvent *event) {
// 	QStyleOption opt;
// 	opt.init(this);
// 	QPainter p(this);
// 	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
// }
