#include "widget.h"

// 다이얼로그 출력 함수
bool CustomerWidget::showCustomerDialog(const QString& title, Customer& customer) {
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    QGridLayout *formLayout = new QGridLayout(&dialog);

    formLayout->addWidget(new QLabel("이름:"), 0, 0);
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setText(QString::fromStdString(customer.name));
    formLayout->addWidget(nameEdit, 0, 1);

    formLayout->addWidget(new QLabel("성별:"), 0, 2);
    QComboBox *genderCombo = new QComboBox(&dialog);
    genderCombo->addItems({"남성", "여성"});
    genderCombo->setCurrentText(QString::fromStdString(customer.gender));
    formLayout->addWidget(genderCombo, 0, 3);

    formLayout->addWidget(new QLabel("나이:"), 1, 0);
    QSpinBox *ageSpinBox = new QSpinBox(&dialog);
    ageSpinBox->setRange(0, 120);
    ageSpinBox->setValue(customer.age);
    formLayout->addWidget(ageSpinBox, 1, 1);

    formLayout->addWidget(new QLabel("전화번호:"), 1, 2);
    QLineEdit *phoneEdit = new QLineEdit(&dialog);
    phoneEdit->setText(QString::fromStdString(customer.phone));
    phoneEdit->setInputMask("999-9999-9999;_");
    phoneEdit->setPlaceholderText("숫자 11자리 입력");
    formLayout->addWidget(phoneEdit, 1, 3);

    formLayout->addWidget(new QLabel("회원분류:"), 2, 0);
    QComboBox *categoryCombo = new QComboBox(&dialog);
    categoryCombo->addItems({"VIP", "일반회원", "신규회원"});
    categoryCombo->setCurrentText(QString::fromStdString(customer.category));
    formLayout->addWidget(categoryCombo, 2, 1);

    formLayout->addWidget(new QLabel("비고:"), 2, 2);
    QLineEdit *noteEdit = new QLineEdit(&dialog);
    noteEdit->setText(QString::fromStdString(customer.note));
    formLayout->addWidget(noteEdit, 2, 3);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    formLayout->addWidget(buttonBox, 3, 0, 1, 4);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "입력 오류", "이름은 필수 입력 항목입니다.");
            return false;
        }
        QString phoneText = phoneEdit->text();
        QString digitsOnly = phoneText;
        digitsOnly.remove("-");
        if (digitsOnly.length() != 11 || phoneText.contains("_")) {
            QMessageBox::warning(this, "입력 오류", "전화번호는 숫자 11자리를 입력해주세요.");
            return false;
        }
        customer.name = nameEdit->text().toStdString();
        customer.gender = genderCombo->currentText().toStdString();
        customer.age = ageSpinBox->value();
        customer.phone = phoneEdit->text().toStdString();
        customer.category = categoryCombo->currentText().toStdString();
        customer.note = noteEdit->text().toStdString();
        return true;
    }
    return false;
}
