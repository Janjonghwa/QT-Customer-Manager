#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

// 고객 정보 구조체
struct Customer {
    std::string name;
    std::string gender;
    int age;
    std::string phone;
    std::string category;
    std::string note; // 비고란 작성

};

#endif // CUSTOMER_H
