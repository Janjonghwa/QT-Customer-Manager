#include "widget.h"

// 검색 다이얼로그 생성 함수
void CustomerWidget::openSearchDialog() {
    QDialog searchDialog(this);
    searchDialog.setWindowTitle("고객 검색");
    QVBoxLayout *mainLayout = new QVBoxLayout(&searchDialog);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *fieldLabel = new QLabel("검색 필드:", &searchDialog);
    QComboBox *fieldCombo = new QComboBox(&searchDialog);
    fieldCombo->addItems({"이름", "성별", "전화번호", "회원분류", "비고"});
    QLabel *textLabel = new QLabel("검색어:", &searchDialog);
    QLineEdit *searchEdit = new QLineEdit(&searchDialog);
    searchLayout->addWidget(fieldLabel);
    searchLayout->addWidget(fieldCombo);
    searchLayout->addWidget(textLabel);
    searchLayout->addWidget(searchEdit);
    mainLayout->addLayout(searchLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &searchDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &searchDialog, &QDialog::reject);

    if (searchDialog.exec() == QDialog::Accepted) {
        QString searchField = fieldCombo->currentText();
        QString searchText = searchEdit->text();
        performSearch(searchField, searchText);
    }
}

// 검색 결과 생성 함수
void CustomerWidget::performSearch(const QString& searchField, const QString& searchText) {
    if (searchText.isEmpty()) {
        updateTable();
        return;
    }
    auto filteredCustomers = model.filterCustomers(searchField, searchText);
    tableWidget->setUpdatesEnabled(false);
    tableWidget->setRowCount(0);
    for (const auto& customer : filteredCustomers) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        addCustomerToTable(row, customer);
    }
    setupTableColumns();
    tableWidget->setUpdatesEnabled(true);
    if (filteredCustomers.empty()) {
        QMessageBox::information(this, "검색 결과", "검색 결과가 없습니다.");
    }
}
