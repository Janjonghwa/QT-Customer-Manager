#include "widget.h"

// 고객 데이터 저장 함수
void CustomerWidget::saveCustomerData() {
    if (model.saveToJson(DATA_FILE_PATH)) {
        qDebug() << "고객 데이터가 성공적으로 저장되었습니다.";
    } else {
        QMessageBox::warning(this, "저장 오류", "고객 데이터를 저장하는 중 오류가 발생했습니다.");
    }
}

// 고객 데이터 불러오기 함수
void CustomerWidget::loadCustomerData() {
    if (QFile::exists(DATA_FILE_PATH)) {
        if (model.loadFromJson(DATA_FILE_PATH)) {
            updateTable();
            qDebug() << "고객 데이터가 성공적으로 불러와졌습니다.";
        } else {
            QMessageBox::warning(this, "불러오기 오류", "고객 데이터를 불러오는 중 오류가 발생했습니다.");
        }
    }
}

// 프로그램 종료 시 이벤트 처리
void CustomerWidget::closeEvent(QCloseEvent *event) {
    saveCustomerData();
    event->accept();
}
