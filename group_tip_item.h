#ifndef GROUP_TIP_ITEM_H
#define GROUP_TIP_ITEM_H

#include "utils/list_item_base.h"
#include <QLabel>

// 群提示项
class GroupTipItem : public ListItemBase {
public:
    GroupTipItem(QWidget* parent = nullptr);
    QSize sizeHint() const override;
    void SetGroupTip(QString str);
private:
    void SetUI();
private:
    QString tip_;//提示信息
private:
    QLabel* tip_label_;
};

#endif//GROUP_TIP_ITEM_H