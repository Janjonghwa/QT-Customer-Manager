#include "widget.h"

// 고객 추가
void CustomerWidget::addCustomer() {
    Customer newCustomer;
    if (showCustomerDialog("고객 추가", newCustomer)) {
        model.addCustomer(newCustomer);
        updateTable();
        QMessageBox::information(this, "추가 완료", "고객 정보가 추가되었습니다.");
    }
}

// 고객 정보 수정
void CustomerWidget::editCustomer() {
    int selectedRow = tableWidget->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "수정 오류", "수정할 고객을 선택해주세요.");
        return;
    }

    // 현재 선택된 셀에서 원본 인덱스 가져오기
    int selectedCol = std::max(0, tableWidget->currentColumn()); // 선택된 열 또는 기본값 0
    QTableWidgetItem* item = tableWidget->item(selectedRow, selectedCol);
    if (!item) {
        QMessageBox::warning(this, "수정 오류", "고객 정보를 찾을 수 없습니다.");
        return;
    }

    int originalIndex = item->data(Qt::UserRole).toInt();
    const auto& customers = model.getAllCustomers();

    if (originalIndex >= static_cast<int>(customers.size())) {
        QMessageBox::warning(this, "수정 오류", "고객 정보를 찾을 수 없습니다.");
        return;
    }

    Customer selectedCustomer = customers[originalIndex];
    if (showCustomerDialog("고객 정보 수정", selectedCustomer)) {
        model.updateCustomer(originalIndex, selectedCustomer);

        // 행의 모든 셀 업데이트
        addCustomerToTable(selectedRow, selectedCustomer);

        // 원본 인덱스 정보 유지
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* updatedItem = tableWidget->item(selectedRow, col);
            if (updatedItem) {
                updatedItem->setData(Qt::UserRole, originalIndex);
            }
        }

        QMessageBox::information(this, "수정 완료", "고객 정보가 수정되었습니다.");
    }
}


// 고객 정보 삭제
void CustomerWidget::deleteCustomer() {
    int selectedRow = tableWidget->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "삭제 오류", "삭제할 고객을 선택해주세요.");
        return;
    }

    // 현재 선택된 셀에서 원본 인덱스 가져오기
    int selectedCol = std::max(0, tableWidget->currentColumn()); // 선택된 열 또는 기본값 0
    QTableWidgetItem* item = tableWidget->item(selectedRow, selectedCol);
    if (!item) {
        QMessageBox::warning(this, "삭제 오류", "고객 정보를 찾을 수 없습니다.");
        return;
    }

    int originalIndex = item->data(Qt::UserRole).toInt();
    const auto& customers = model.getAllCustomers();

    if (originalIndex >= static_cast<int>(customers.size())) {
        QMessageBox::warning(this, "삭제 오류", "고객 정보를 찾을 수 없습니다.");
        return;
    }

    // 삭제 확인 메시지에 고객 이름 표시
    Customer selectedCustomer = customers[originalIndex];
    QString customerName = QString::fromStdString(selectedCustomer.name);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "삭제 확인",
        QString("'%1' 고객의 정보를 삭제하시겠습니까?").arg(customerName),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        model.removeCustomer(originalIndex);
        updateTable(); // 삭제 후에는 테이블 전체 갱신이 필요합니다
        QMessageBox::information(this, "삭제 완료", "고객 정보가 삭제되었습니다.");
    }
}

// 전체 고객 수 표기 다이얼로그
void CustomerWidget::displayAllCustomers() {

    // 정렬 상태 초기화
    sortColumn = -1;
    sortOrder = true;

    // 테이블 헤더의 정렬 표시기 제거
    tableWidget->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);

    // 테이블 업데이트
    updateTable();

    // 고객 수 표시
    if (model.isEmpty()) {
        QMessageBox::information(this, "정보", "등록된 고객이 없습니다.");
    } else {
        QMessageBox::information(this, "전체 출력",
                                 QString("총 %1명의 고객 정보가 출력되었습니다.").arg(model.getAllCustomers().size()));
    }
}

// 테이블 더블 클릭 시 해당 고객 정보 수정하는 다이얼로그 열기
void CustomerWidget::onTableDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    int row = index.row();
    int col = index.column();

    QTableWidgetItem* item = tableWidget->item(row, col);
    if (!item) return;

    int originalIndex = item->data(Qt::UserRole).toInt();
    const auto& customers = model.getAllCustomers();

    if (originalIndex >= static_cast<int>(customers.size())) return;

    Customer selectedCustomer = customers[originalIndex];
    if (showCustomerDialog("고객 정보 보기 및 수정", selectedCustomer)) {
        model.updateCustomer(originalIndex, selectedCustomer);

        // 행의 모든 셀 업데이트
        addCustomerToTable(row, selectedCustomer);

        // 원본 인덱스 정보 유지
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* updatedItem = tableWidget->item(row, col);
            if (updatedItem) {
                updatedItem->setData(Qt::UserRole, originalIndex);
            }
        }

        QMessageBox::information(this, "수정 완료", "고객 정보가 수정되었습니다.");
    }
}
