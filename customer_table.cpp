#include "widget.h"

// 테이블 생성 함수
void CustomerWidget::setupTable(QVBoxLayout *mainLayout) {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels({"이름", "성별", "나이", "전화번호", "회원분류", "비고"});
    tableWidget->horizontalHeader()->setStyleSheet("font-weight: bold;font-size: 12px;");
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 모든 구분선(가로 및 세로) 제거
    tableWidget->setShowGrid(false);

    tableWidget->setAlternatingRowColors(true);

    // 홀수/짝수 행 색상 차이 줄이기
    QPalette p = tableWidget->palette();
    p.setColor(QPalette::Base, QColor("#F8FBFF"));
    p.setColor(QPalette::AlternateBase, QColor("#F5F9FD"));
    tableWidget->setPalette(p);

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setHighlightSections(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(tableWidget->horizontalHeader(), &QHeaderView::sectionClicked, this, &CustomerWidget::sortTable);
    connect(tableWidget, &QTableWidget::doubleClicked, this, &CustomerWidget::onTableDoubleClicked);

    mainLayout->addWidget(tableWidget);
}


// 테이블 업데이트 함수
void CustomerWidget::updateTable() {
    tableWidget->setUpdatesEnabled(false);
    tableWidget->setRowCount(0);
    const auto& customers = model.getAllCustomers();

    // 만약 정렬이 활성화되어 있다면 정렬된 순서로 표시
    if (sortColumn >= 0) {
        // 원본 인덱스와 데이터를 함께 저장
        std::vector<std::pair<int, Customer>> indexedCustomers;
        for (size_t i = 0; i < customers.size(); ++i) {
            indexedCustomers.push_back({static_cast<int>(i), customers[i]});
        }

        // 인덱스를 함께 정렬
        std::sort(indexedCustomers.begin(), indexedCustomers.end(),
                  [this](const auto& a, const auto& b) {
                      return model.compareCustomers(a.second, b.second, sortColumn, sortOrder);
                  });

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

                // 중요: 모든 셀에 원본 인덱스 저장
                items[i]->setData(Qt::UserRole, originalIndex);

                tableWidget->setItem(row, i, items[i]);
            }
        }
    } else {
        // 정렬이 없는 경우 순서대로 표시
        for (size_t i = 0; i < customers.size(); ++i) {
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);

            // 각 셀 생성 및 데이터 설정
            QTableWidgetItem *items[6] = {
                new QTableWidgetItem(QString::fromStdString(customers[i].name)),
                new QTableWidgetItem(QString::fromStdString(customers[i].gender)),
                new QTableWidgetItem(QString::number(customers[i].age)),
                new QTableWidgetItem(QString::fromStdString(customers[i].phone)),
                new QTableWidgetItem(QString::fromStdString(customers[i].category)),
                new QTableWidgetItem(QString::fromStdString(customers[i].note))
            };

            // 셀 정렬 및 원본 인덱스 저장
            for (int j = 0; j < 6; j++) {
                if (j < 5) {
                    items[j]->setTextAlignment(Qt::AlignCenter);
                }

                // 모든 셀에 원본 인덱스 저장
                items[j]->setData(Qt::UserRole, static_cast<int>(i));

                tableWidget->setItem(row, j, items[j]);
            }
        }
    }

    setupTableColumns();
    tableWidget->setUpdatesEnabled(true);

    // 총 고객 수 업데이트
    if (countLabel) {
        countLabel->setText(QString("총 고객: %1명").arg(customers.size()));
    }
}

// 고객 추가후 테이블에 반영
void CustomerWidget::addCustomerToTable(int row, const Customer& customer) {
    QTableWidgetItem *items[6] = {
        new QTableWidgetItem(QString::fromStdString(customer.name)),
        new QTableWidgetItem(QString::fromStdString(customer.gender)),
        new QTableWidgetItem(QString::number(customer.age)),
        new QTableWidgetItem(QString::fromStdString(customer.phone)),
        new QTableWidgetItem(QString::fromStdString(customer.category)),
        new QTableWidgetItem(QString::fromStdString(customer.note))
    };

    // 비고 제외 가운데 정렬
    for (int i = 0; i < 5; i++) {
        items[i]->setTextAlignment(Qt::AlignCenter);
    }

    for (int i = 0; i < 6; i++) {
        tableWidget->setItem(row, i, items[i]);
        // 인덱스 저장은 이 함수에서 하지 않음 - updateTable 또는 sortTable에서 처리
    }
}

// 테이블 가로 너비 조절
void CustomerWidget::setupTableColumns() {
    const int columnWidths[6] = {70, 50, 50, 120, 80, 300};
    for (int i = 0; i < 6; i++) {
        tableWidget->setColumnWidth(i, columnWidths[i]);
    }
}
