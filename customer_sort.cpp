#include "widget.h"
#include <algorithm>

void CustomerWidget::sortTable(int column) {
    // 비고 열이 클릭되었을 때 특별 처리
    if (column == 5) {
        if (column == sortColumn) {
            // 이미 비고 열이 선택된 상태에서 다시 클릭하면 정렬 순서만 변경
            sortOrder = !sortOrder;
        } else {
            // 비고 열을 처음 클릭했을 때는 내림차순(false)으로 시작
            sortColumn = column;
            sortOrder = false; // 내림차순 (가나다 역순)
        }
    } else {
        // 다른 열은 기존 로직대로 처리
        if (column == sortColumn) {
            sortOrder = !sortOrder;
        } else {
            sortColumn = column;
            sortOrder = true; // 다른 열은 기본적으로 오름차순
        }
    }

    // 정렬 표시 설정
    tableWidget->horizontalHeader()->setSortIndicator(column, sortOrder ? Qt::AscendingOrder : Qt::DescendingOrder);

    // 원본 데이터와 인덱스 함께 정렬
    const auto& originalCustomers = model.getAllCustomers();
    std::vector<std::pair<int, Customer>> indexedCustomers;

    // 원본 인덱스와 데이터를 함께 저장
    for (size_t i = 0; i < originalCustomers.size(); ++i) {
        indexedCustomers.push_back({static_cast<int>(i), originalCustomers[i]});
    }

    // 인덱스를 함께 정렬
    std::sort(indexedCustomers.begin(), indexedCustomers.end(),
              [this, column](const auto& a, const auto& b) {
                  return model.compareCustomers(a.second, b.second, column, sortOrder);
              });

    // 테이블 내용을 지우고 다시 채움
    tableWidget->setUpdatesEnabled(false);
    tableWidget->setRowCount(0);

    // 정렬된 순서로 테이블에 추가하면서 모든 셀에 원본 인덱스를 저장
    for (const auto& [originalIndex, customer] : indexedCustomers) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // 각 셀 생성 및 데이터 설정
        QTableWidgetItem *items[6] = {
            new QTableWidgetItem(QString::fromStdString(customer.name)),
            new QTableWidgetItem(QString::fromStdString(customer.gender)),
            new QTableWidgetItem(QString::number(customer.age)),
            new QTableWidgetItem(QString::fromStdString(customer.phone)),
            new QTableWidgetItem(QString::fromStdString(customer.category)),
            new QTableWidgetItem(QString::fromStdString(customer.note))
        };

        // 셀 정렬 및 원본 인덱스 저장
        for (int i = 0; i < 6; i++) {
            if (i < 5) {
                items[i]->setTextAlignment(Qt::AlignCenter);
            }

            // 모든 셀에 원본 인덱스 저장
            items[i]->setData(Qt::UserRole, originalIndex);

            tableWidget->setItem(row, i, items[i]);
        }
    }

    setupTableColumns();
    tableWidget->setUpdatesEnabled(true);
}

