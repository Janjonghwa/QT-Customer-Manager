#include "widget.h"

// 메뉴 버튼 생성
void CustomerWidget::setupButtons(QVBoxLayout *mainLayout) {
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // 버튼 생성
    addButton = createButton(" 고객 추가", &CustomerWidget::addCustomer);
    deleteButton = createButton(" 고객 삭제", &CustomerWidget::deleteCustomer);
    editButton = createButton(" 고객 수정", &CustomerWidget::editCustomer);
    displayButton = createButton(" 전체 출력", &CustomerWidget::displayAllCustomers);
    searchButton = createButton(" 고객 검색", &CustomerWidget::openSearchDialog);

    // 레이아웃에 버튼 추가
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(displayButton);
    buttonLayout->addWidget(searchButton);

    mainLayout->addLayout(buttonLayout);
}

// 메뉴 버튼
QPushButton* CustomerWidget::createButton(const QString &text, void (CustomerWidget::*slot)()) {
    QPushButton *button = new QPushButton(text, this);

    // Set icons based on button type
    if (text == " 고객 추가")
        button->setIcon(QIcon(":/icons/add.png"));
    else if (text == " 고객 삭제")
        button->setIcon(QIcon(":/icons/delete.png"));
    else if (text == " 고객 수정")
        button->setIcon(QIcon(":/icons/edit.png"));
    else if (text == " 전체 출력")
        button->setIcon(QIcon(":/icons/refresh.png"));
    else if (text == " 고객 검색")
        button->setIcon(QIcon(":/icons/search.png"));

    button->setIconSize(QSize(24, 24));
    button->setFixedHeight(36);
    button->setCursor(Qt::PointingHandCursor);
    button->setFlat(true);

    // 메뉴 버튼 폰트 설정
    QFont buttonFont = button->font();
    buttonFont.setPixelSize(12);
    button->setFont(buttonFont);

    // 메뉴 버튼 연결
    button->disconnect();
    connect(button, &QPushButton::clicked, this, slot);

    return button;
}
