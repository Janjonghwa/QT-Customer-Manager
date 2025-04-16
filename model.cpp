#include "model.h"

// 모델 생성자 함수
CustomerModel::CustomerModel() {
    // 기본 데이터 생성
    customers.push_back({"요시", "남성", 25, "010-1111-2222","VIP" ,"매우 귀여움"});
    customers.push_back({"마리오", "여성", 23, "010-3333-4444","일반회원" ,"수염이 멋있는 슈퍼마리오"});
    customers.push_back({"루이지", "남성", 27, "010-5555-6666","일반회원" ,"형인 마리오와 매우 친함"});
    customers.push_back({"부끄부끄", "여성", 29, "010-7777-8888","일반회원","부끄러움이 많은 유령"});
    customers.push_back({"쿠파", "남성", 22, "010-9999-0000","신규회원","강한 상남자"});
}

// 고객 추가
void CustomerModel::addCustomer(const Customer& customer) {
    customers.push_back(customer);
}

// index에 해당하는 고객 삭제
void CustomerModel::removeCustomer(int index) {
    if (index >= 0 && index < customers.size()) {
        customers.erase(customers.begin() + index);
    }
}

// 전체 데이터 리턴
const std::vector<Customer>& CustomerModel::getAllCustomers() const {
    return customers;
}

// 저장 데이터 여부 확인
bool CustomerModel::isEmpty() const {
    return customers.empty();
}

// 추가된 함수
void CustomerModel::updateCustomer(int index, const Customer& customer) {
    if (index >= 0 && index < customers.size()) {
        customers[index] = customer;
    }
}

// 총 고객 수 리턴 함수
int CustomerModel::totalCustomer() {
    return customers.size();
}

// 데이터 정렬을 위한 데이터 비교 함수
bool CustomerModel::compareCustomers(const Customer& a, const Customer& b, int column, bool ascending) {
    bool result = false;

    switch (column) {
    case 0: // 이름
        result = a.name < b.name;
        break;
    case 1: // 성별
        result = a.gender < b.gender;
        break;
    case 2: // 나이
        result = a.age < b.age;
        break;
    case 3: // 전화번호
        result = a.phone < b.phone;
        break;
    case 4: // 회원분류
        result = a.category < b.category;
        break;
    case 5: // 비고
        result = a.note < b.note;
        break;
    default:
        return false;
    }

    return ascending ? result : !result;
}

// 검색 로직 (데이터 필터링)
std::vector<Customer> CustomerModel::filterCustomers(const QString& searchField, const QString& searchText)
{
    std::vector<Customer> result;

    const std::map<QString, int> fieldMap = {
        {"이름", 0},
        {"성별", 1},
        {"전화번호", 3},
        {"회원분류", 4},
        {"비고", 5}
    };

    int fieldIndex = fieldMap.at(searchField);
    const auto& customers = getAllCustomers();

    for (const auto& customer : customers) {
        QString value;
        switch (fieldIndex) {
        case 0: value = QString::fromStdString(customer.name); break;
        case 1: value = QString::fromStdString(customer.gender); break;
        case 3: value = QString::fromStdString(customer.phone); break;
        case 4: value = QString::fromStdString(customer.category); break;
        case 5: value = QString::fromStdString(customer.note); break;
        }

        // 검색값이 value에 포함되어 있는지 확인
        if (value.contains(searchText, Qt::CaseInsensitive)) {
            result.push_back(customer);
        }
    }

    return result;
}

// 고객 정보를 JSON 파일로 저장하는 함수
bool CustomerModel::saveToJson(const QString& filePath) const {
    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("저장 파일을 열 수 없습니다.");
        return false;
    }

    QJsonObject rootObject;
    QJsonArray customersArray;

    // 모든 고객 정보를 JSON 배열로 변환
    for (const auto& customer : customers) {
        QJsonObject customerObject;
        customerObject["name"] = QString::fromStdString(customer.name);
        customerObject["gender"] = QString::fromStdString(customer.gender);
        customerObject["age"] = customer.age;
        customerObject["phone"] = QString::fromStdString(customer.phone);
        customerObject["category"] = QString::fromStdString(customer.category);
        customerObject["note"] = QString::fromStdString(customer.note);

        customersArray.append(customerObject);
    }

    rootObject["customers"] = customersArray;
    QJsonDocument saveDoc(rootObject);

    saveFile.write(saveDoc.toJson());
    saveFile.close();

    return true;
}

// JSON 파일에서 고객 정보를 불러오는 함수
bool CustomerModel::loadFromJson(const QString& filePath) {
    QFile loadFile(filePath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("저장 파일을 열 수 없습니다.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonParseError parseError;
    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning("JSON 파싱 오류: %s", qPrintable(parseError.errorString()));
        return false;
    }

    if (!loadDoc.isObject()) {
        qWarning("JSON 문서가 객체가 아닙니다.");
        return false;
    }

    QJsonObject rootObject = loadDoc.object();
    QJsonArray customersArray = rootObject["customers"].toArray();

    // 기존 고객 정보 초기화
    customers.clear();

    // JSON 배열에서 고객 정보 불러오기
    for (const QJsonValue& value : customersArray) {
        QJsonObject customerObject = value.toObject();

        Customer customer;
        customer.name = customerObject["name"].toString().toStdString();
        customer.gender = customerObject["gender"].toString().toStdString();
        customer.age = customerObject["age"].toInt();
        customer.phone = customerObject["phone"].toString().toStdString();
        customer.category = customerObject["category"].toString().toStdString();
        customer.note = customerObject["note"].toString().toStdString();

        customers.push_back(customer);
    }

    loadFile.close();
    return true;
}
