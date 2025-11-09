#ifndef SEARCH_USER_ITEM_H
#define SEARCH_USER_ITEM_H
#include <QLabel>
#include <memory>

#include "user_data.h"
#include "utils/list_item_base.h"


// 搜索的用户 item
class SearchUserItem : public ListItemBase {
	Q_OBJECT
public:
	explicit SearchUserItem(QWidget* parent = nullptr);
	~SearchUserItem();
    void SetInfo(std::shared_ptr<SearchInfo> search_info);
	std::shared_ptr<SearchInfo> GetInfo();
private:
	void SetUI();
private:
    std::shared_ptr<SearchInfo> search_info_;
private:
	QLabel* user_icon_;	  // 用户头像
	QLabel* name_label_;  // 用户名
};

#endif	// SEARCH_USER_ITEM_H