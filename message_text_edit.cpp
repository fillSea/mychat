#include "message_text_edit.h"

MessageTextEdit::MessageTextEdit(QWidget *parent) : QTextEdit(parent) {
	this->setMaximumHeight(60);
}

MessageTextEdit::~MessageTextEdit() {}

// 获取编写在文本编辑框中的消息列表
// 该函数从文本编辑框中提取混合内容(文本和非文本对象如图片)，并整理成结构化的消息列表
QVector<MsgInfo> MessageTextEdit::GetMsgList() {
    // 清空结果消息列表，准备存储提取的消息
    get_msg_list_.clear();

    // 获取文档的纯文本内容
    QString doc = this->document()->toPlainText();
    // 用于累积文本内容的临时变量
    QString text = "";
    // 用于跟踪当前处理到的非文本消息索引
    int indexUrl = 0;
    // 原始消息列表的总数量
    int count = msg_list_.size();

    // 遍历文档中的每个字符
    for (int index = 0; index < doc.size(); index++) {
        // 当遇到对象替换字符时(标记非文本内容的位置)
        if (doc[index] == QChar::ObjectReplacementCharacter) {
            // 如果当前累积的文本不为空，将其插入到结果消息列表中
            if (!text.isEmpty()) {
                QPixmap pix; // 对于纯文本消息，pix为空
                InsertMsgList(get_msg_list_, "text", text, pix); // 插入文本消息
                text.clear(); // 清空累积的文本
            }
            
            // 查找并添加对应的非文本消息(如图片等)
            while (indexUrl < count) {
                MsgInfo msg = msg_list_[indexUrl];
                // 通过检查HTML内容来确定是否匹配当前位置的非文本对象
                if (this->document()->toHtml().contains(msg.content, Qt::CaseSensitive)) {
                    indexUrl++; // 移动到下一个非文本消息
                    get_msg_list_.append(msg); // 添加到结果消息列表
                    break; // 跳出循环，继续处理下一个文档字符
                }
                indexUrl++; // 继续查找下一个可能匹配的非文本消息
            }
        } else {
            // 如果是普通字符，添加到累积的文本中
            text.append(doc[index]);
        }
    }
    
    // 确保处理完文档后，最后一段累积的文本也被添加到消息列表中
    if (!text.isEmpty()) {
        QPixmap pix;
        InsertMsgList(get_msg_list_, "text", text, pix);
        text.clear();
    }
    
    // 清空原始消息列表和文本编辑框内容，准备下一轮输入
    msg_list_.clear();
    this->clear();
    
    // 返回整理好的消息列表
    return get_msg_list_;
}

void MessageTextEdit::InsertMsgList(QVector<MsgInfo> &list, QString flag, QString text,
									QPixmap pix) {
	MsgInfo msg;
	msg.msg_flag = flag;
	msg.content = text;
	msg.pixmap = pix;
	list.append(msg);
}