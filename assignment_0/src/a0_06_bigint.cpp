#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>  // 用于isdigit

class BigInt {
private:
    std::string digits;  // 逆序存储

    // 辅助函数：移除前导零
    void removeLeadingZeros() {
        // 逆序存储时，前导零在字符串末尾
        while (digits.size() > 1 && digits.back() == '0') {
            digits.pop_back();
        }
    }

public:
    // 默认构造：值为0
    BigInt() : digits("0") {}

    // 从字符串构造
    BigInt(const std::string& s) {
        // 检查是否为空
        if (s.empty()) {
            digits = "0";
            return;
        }

        // 检查是否全为数字
        for (char c : s) {
            if (!std::isdigit(c)) {
                digits = "0";  // 无效输入，设为0
                return;
            }
        }

        // 跳过前导零
        int start = 0;
        while (start < s.size() && s[start] == '0') {
            start++;
        }

        // 如果全是零
        if (start == s.size()) {
            digits = "0";
            return;
        }

        // 逆序存储
        digits = s.substr(start);
        std::reverse(digits.begin(), digits.end());
    }

    // 从C字符串构造
    BigInt(const char* s) : BigInt(std::string(s)) {}

    // 加法运算符
    BigInt operator+(const BigInt& other) const {
        BigInt result;
        result.digits.clear();  // 清空默认的"0"

        const std::string& a = this->digits;
        const std::string& b = other.digits;

        int i = 0;
        int carry = 0;

        while (i < a.size() || i < b.size() || carry > 0) {
            int digit_a = (i < a.size()) ? (a[i] - '0') : 0;
            int digit_b = (i < b.size()) ? (b[i] - '0') : 0;

            int sum = digit_a + digit_b + carry;
            result.digits.push_back((sum % 10) + '0');
            carry = sum / 10;

            i++;
        }

        return result;
    }

    // 输出运算符
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
        std::string output = num.digits;
        std::reverse(output.begin(), output.end());
        os << output;
        return os;
    }
};

int main() {
    std::string num1_str, num2_str;

    // 读取两行
    std::getline(std::cin, num1_str);
    std::getline(std::cin, num2_str);

    // 创建大整数对象
    BigInt num1(num1_str);
    BigInt num2(num2_str);

    // 相加
    BigInt sum = num1 + num2;

    // 输出
    std::cout << sum << std::endl;

    return 0;
}
