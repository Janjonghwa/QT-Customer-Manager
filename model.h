#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <map>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "customer.h"

class CustomerModel {
public:
    CustomerModel();
    int totalCustomer();
    void addCustomer(const Customer& customer);
    void removeCustomer(int index);
    void updateCustomer(int index, const Customer& customer);
    bool isEmpty() const;
    bool compareCustomers(const Customer& a, const Customer& b, int column, bool ascending);
    const std::vector<Customer>& getAllCustomers() const;
    std::vector<Customer> filterCustomers(const QString& searchField, const QString& searchText);

    // 추가할 함수
    bool saveToJson(const QString& filePath) const;
    bool loadFromJson(const QString& filePath);

private:
    std::vector<Customer> customers;
};

#endif // MODEL_H
