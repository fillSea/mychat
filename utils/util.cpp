#include "util.h"
#include <QStyle>

/**
 * @brief 刷新Qt控件样式的函数对象
 * 
 * 此函数通过先清除再重新应用控件样式的方式，强制刷新Qt控件的外观显示。
 * 在动态修改样式表、控件状态变更或自定义控件样式时特别有用。
 * 
 * @param w 需要刷新样式的QWidget指针
 * 
 * 使用场景：
 * 1. 动态修改控件样式表后需要立即生效
 * 2. 控件状态改变（如禁用/启用、选中状态）但样式未自动更新
 * 3. 自定义控件样式需要强制刷新
 * 4. 复杂嵌套控件的样式未正确传递到子控件
 */
std::function<void(QWidget*)> repolish = [](QWidget *w) {
    // 第一步：从控件中移除当前应用的样式信息，清除样式缓存
    w->style()->unpolish(w);
    
    // 第二步：重新为控件应用样式信息，强制更新控件外观
    w->style()->polish(w);

    w->update();
};


/**
 * @brief 字符串异或加密/解密函数
 * 
 * 此函数通过异或(XOR)操作对输入的QString进行简单加密或解密。
 * 由于异或操作的特性，使用相同的参数再次调用此函数可以恢复原始字符串。
 * 加密密钥基于输入字符串的长度动态生成，增加了一定的安全性。
 * 
 * @param input 需要加密/解密的原始字符串
 * @return QString 处理后的字符串（加密或解密结果）
 * 
 * 使用场景：
 * 1. 简单的数据加密，如配置文件中的敏感信息
 * 2. 临时数据保护，防止直接查看
 * 3. 客户端与服务器之间的简单数据混淆
 * 
 * 注意事项：
 * - 这是一种基础的加密方式，不适用于高安全性要求的场景
 * - 加密和解密使用相同的函数，只需调用两次即可恢复原文
 * - 函数假设字符为ASCII编码范围内的字符
 */
std::function<QString(QString)> xorString = [](QString input){
    QString result = input; // 复制原始字符串，以便进行修改
    int length = input.length(); // 获取字符串的长度
    
    // 生成基于字符串长度的异或码，范围限制在0-254之间
    ushort xor_code = length % 255;
    
    // 遍历字符串中的每个字符，执行异或操作
    for (int i = 0; i < length; ++i) {
        // 获取字符的Unicode值，与异或码进行异或运算，然后转换回QChar
        // 注意：这里假设字符都是ASCII，因此直接转换为QChar
        result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ xor_code));
    }
    
    // 返回加密/解密后的结果
    return result;
};