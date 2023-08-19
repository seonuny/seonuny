#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

int main() {
    string ipv6_hex_str;
    int ipv6_hex[8]; // IPv6 주소를 저장할 정수형 배열
    
    cout << "IPv6 주소를 16진수 문자열로 입력하세요: ";
    cin >> ipv6_hex_str;
    
    // IPv6 hexa 문자열을 16진수 정수형 배열로 변환
    stringstream ss(ipv6_hex_str);
    for (int i = 0; i < 8; i++) {
        ss >> hex >> ipv6_hex[i];
    }
    
    // IPv6 hexa 배열의 맨 마지막 값에서부터 1식 증가시킴
    for (int i = 7; i >= 0; i--) {
        ipv6_hex[i]++;
        if (ipv6_hex[i] <= 0xffff) {
            break;
        }
        ipv6_hex[i] = 0;
    }
    
    // 16진수 정수형 배열을 IPv6 hexa 문자열로 변환하여 출력
    stringstream ss2;
    for (int i = 0; i < 8; i++) {
        ss2 << setfill('0') << setw(4) << hex << ipv6_hex[i];
        if (i < 7) {
            ss2 << ":";
        }
    }
    string ipv6_hex_str2 = ss2.str();
    cout << "IPv6 주소: " << ipv6_hex_str2 << endl;
    
    return 0;
}

