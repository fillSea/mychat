#ifndef CONST_H
#define CONST_H
#include <QString>
#include <QPixmap>

// UI界面状态
enum class UIStatus{
    LOGIN, // 登录
    REGISTER, // 注册
    RESET,
    CHAT
};

// 请求错误返回码
enum class ErrorCodes{
    SUCCESS = 0,
    ERRJSON = 1, // json解析失败
    ERRNETWORK = 2, // 网络错误
};

// net 请求 Id
enum class ReqId{
    GET_VARIFY_CODE = 1001, //获取验证码
    REG_USER = 1002, //注册用户
    RESET_PWD = 1003, //重置密码
    LOGIN_USER = 1004, //用户登录
    CHAT_LOGIN = 1005, //登陆聊天服务器
    CHAT_LOGIN_RSP= 1006, //登陆聊天服务器回包
    SEARCH_USER_REQ = 1007, //用户搜索请求
    SEARCH_USER_RSP = 1008, //搜索用户回包
    ADD_FRIEND_REQ = 1009,  //添加好友申请
    ADD_FRIEND_RSP = 1010, //申请添加好友回复
    NOTIFY_ADD_FRIEND_REQ = 1011,  //通知用户添加好友申请
    AUTH_FRIEND_REQ = 1013,  //认证好友请求
    AUTH_FRIEND_RSP = 1014,  //认证好友回复
    NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    TEXT_CHAT_MSG_REQ  = 1017,  //文本聊天信息请求
    TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
    NOTIFY_OFF_LINE_REQ = 1021, //通知用户下线
    HEART_BEAT_REQ = 1023,      //心跳请求
    HEARTBEAT_RSP = 1024,       //心跳回复
};


enum class Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

// 注册界面提示错误
enum class TipErr{
    SUCCESS = 0,
    ERR_USERNAME = 1,
    ERR_EMAIL = 2,
    ERR_PASSWORD = 3,
    ERR_CONFIRM_PASSWORD = 4,
    ERR_VERIFYCODE = 5,
};




// 点击标签状态
enum ClickLbState{
    Normal = 0,
    Selected = 1
};

// http 请求前缀
const QString gate_url_prefix = "http://localhost:8080";


//聊天界面几种模式
enum ChatUIMode{
    SearchMode, //搜索模式
    ChatMode, //聊天模式
    ContactMode, //联系模式
    SettingsMode, //设置模式
};

//自定义QListWidgetItem的几种类型
enum ListItemType{
    CHAT_USER_ITEM, //聊天用户
    CONTACT_USER_ITEM, //联系人用户
    SEARCH_USER_ITEM, //搜索到的用户
    ADD_USER_TIP_ITEM, //提示添加用户
    INVALID_ITEM,  //不可点击条目
    GROUP_TIP_ITEM, //分组提示条目
    LINE_ITEM,  //分割线
    APPLY_FRIEND_ITEM, //好友申请
};

// 聊天角色
enum class ChatRole{
    Self,
    Other
};


// ChatServer结构体
struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    QString Uid;
};

// 聊天消息结构体
struct MsgInfo{
    QString msg_flag;// 消息类型, "text,image,file"
    QString content;//表示文件和图像的url,文本信息
    QPixmap pixmap;//文件和图片的缩略图
};


// 初始头像列表
const std::vector<QString> heads = {
    "../res/head_1.jpg",
    "../res/head_2.jpg",
    "../res/head_3.jpg",
    "../res/head_4.jpg",
    "../res/head_5.jpg"
};

// 测试数据
const std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year, new life",
                            "You have to love yourself",
                            "My love is written in the wind ever since the whole world is you"};


const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};








#endif