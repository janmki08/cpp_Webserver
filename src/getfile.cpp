#include "getfile.h"

using namespace std;

// TODO
// MIME 타입 결정 추가
string get_file(const string &path)
{
    ifstream file(path); // 파일 열기
    if (!file.is_open())
        return ""; // 열기 실패

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}