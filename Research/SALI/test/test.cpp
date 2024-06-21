#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getCurrentDateTime() {
    // 現在の時刻を取得
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    // 日付時刻をフォーマット
    std::ostringstream oss;
    oss << std::put_time(ltm, "%y%m%d_%H%M");
    return oss.str();
}

void writeToFile(const std::string& fileName) {
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "ファイルを開くことができませんでした: " << fileName << std::endl;
        return;
    }

    // ファイルに書き込む内容
    outFile << "列1,列2,列3" << std::endl;
    outFile << "データ1,データ2,データ3" << std::endl;

    outFile.close();
    std::cout << "ファイルにデータを書き込みました: " << fileName << std::endl;
}

int main() {
    std::string dateTime = getCurrentDateTime();
    std::string fileName = dateTime + ".csv";

    writeToFile(fileName);

    return 0;
}
