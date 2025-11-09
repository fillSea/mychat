#ifndef SEARCH_LIST_H
#define SEARCH_LIST_H
#include <QListWidget>

// 搜索结果列表
class SearchList: public QListWidget{
    Q_OBJECT
public:
    SearchList(QWidget *parent = nullptr);
    // void CloseFindDlg();
    // void SetSearchEdit(QWidget* edit);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    // void waitPending(bool pending = true);
    // bool _send_pending;
    // void addTipItem();
    // std::shared_ptr<QDialog> _find_dlg;
    // QWidget* _search_edit;
    // LoadingDlg * _loadingDialog;
private slots:
    void SlotItemClicked(QListWidgetItem *item);
    // void slot_user_search(std::shared_ptr<SearchInfo> si);
signals:
    void SigLoadingSearchUser();
    // void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
};

#endif//SEARCH_LIST_H