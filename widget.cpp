#include "widget.h"
#include <QApplication>

// 생성자
CustomerWidget::CustomerWidget(QWidget *parent)
    : QWidget(parent), sortColumn(-1), sortOrder(true)
{
    setupUI();

    // 저장된 데이터 불러오기
    loadCustomerData();

    updateTable();
}

// 초기 화면 세팅 (생성자에서 한번만 불려짐)
void CustomerWidget::setupUI() {
    // 전체 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 최상단 버튼 세팅 함수
    setupButtons(mainLayout);

    // 테이블 세팅 함수
    setupTable(mainLayout);

    QHBoxLayout *themeLayout = new QHBoxLayout();

    // 총 고객수 표기 라벨 생성
    countLabel = new QLabel(this);
    countLabel->setText("총 고객: 0명");
    countLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    themeLayout->addWidget(countLabel);

    // 정렬을 위해 가운데 여백 생성
    themeLayout->addStretch();

    // 테마 선택 콤보박스 생성
    QLabel *themeLabel = new QLabel(" 테마 선택 : ", this);
    themeLabel->setStyleSheet("font-weight: bold;font-size: 12pt;");
    themeCombo = new QComboBox(this);

    // 아이콘 추가
    QIcon blueIcon(":/icons/blue_theme.png");
    QIcon greenIcon(":/icons/green_theme.png");
    QIcon darkIcon(":/icons/dark_theme.png");

    themeCombo->addItem(blueIcon, " 파란색");
    themeCombo->addItem(greenIcon, " 초록색");
    themeCombo->addItem(darkIcon, " 다크모드");

    themeCombo->setFixedWidth(117); // 너비 고정
    themeCombo->setFixedHeight(30); // 높이 고정

    // 콤보박스 변경에 따라 테마 설정
    connect(themeCombo, &QComboBox::currentTextChanged, this, &CustomerWidget::changeTheme);

    themeLayout->addStretch();
    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(themeCombo);
    mainLayout->addLayout(themeLayout);

    // 위젯 윈도우 기본 설정 (타이틀, 아이콘, 크기)
    setWindowTitle("고객 관리 프로그램");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setMinimumSize(800, 500);

    // 기본 테마 적용
    changeTheme(" 파란색"); // 프로그램 시작 시 기본 테마 적용
}

// 테마 변경 함수
void CustomerWidget::changeTheme(const QString &theme) {
    applyTheme(theme);
}

// 테마 적용 함수
void CustomerWidget::applyTheme(const QString &theme) {
    // 테마 구조체 정의
    struct ThemeColors {
        QString primaryColor;
        QString secondaryColor;
        QString bgColorLight;
        QString bgColorDark;
        QString tableAlternateBg;
        QString tableBg;
        QString tableGrid;
        QString tableBorder;
        QString tableSelectedBg;
        QString textColor;
        QString headerTextColor;
        QString buttonTextColor;
        QString comboBorder;
        QString comboBg;
        QString inputBg;
        QString inputTextColor;
        QString hoverColor;
        QString pressedColor;
    };

    // 테마별 색상 정의
    QMap<QString, ThemeColors> themes;

    // 파란색 테마
    themes[" 파란색"] = {
        "#4682B4", "#5C97BF", "#F0F8FF", "#E6F2FF",
        "#EBF5FF", "#F8FBFF", "#D0E1F2", "#B0C4DE",
        "#B0D4FF", "#333333", "white", "white",
        "#B0C4DE", "#F0F8FF", "#FFFFFF", "#333333",
        "#6CA7CF", "#3A6A8E"
    };

    // 초록색 테마
    themes[" 초록색"] = {
        "#4CAF50", "#5DBE64", "#F0FFF0", "#E8F5E9",
        "#EDF7EE", "#F8FFF8", "#C8E6C9", "#A5D6A7",
        "#B9E6B8", "#333333", "white", "white",
        "#A5D6A7", "#F0FFF0", "#FFFFFF", "#333333",
        "#76CB7A", "#3B8C3F"
    };

    // 다크모드 테마
    themes[" 다크모드"] = {
        "#424242", "#505050", "#2E2E2E", "#252525",
        "#353535", "#2E2E2E", "#505050", "#505050",
        "#505050", "#E0E0E0", "#E0E0E0", "#E0E0E0",
        "#505050", "#383838", "#383838", "#E0E0E0",
        "#606060", "#303030"
    };

    // 선택된 테마 가져오기
    ThemeColors colors = themes[theme];

    // 폰트 스타일 설정
    QFont appFont = QApplication::font();
    QString fontFamily = appFont.family();
    int fontSizePt = appFont.pointSize() > 0 ? appFont.pointSize() : 10;

    QString baseFontStyle = QString("font-family: '%1'; font-size: %2pt;").arg(fontFamily).arg(fontSizePt);
    QString headerFontStyle = QString("font-family: '%1'; font-size: %2pt; font-weight: bold;").arg(fontFamily).arg(fontSizePt );

    // 스타일 문자열 구성 - 모든 테마에 공통적인 형식
    QString windowStyle = QString("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2); color: %3; %4")
                              .arg(colors.bgColorLight, colors.bgColorDark, colors.textColor, baseFontStyle);

    QString tableStyle = QString(
                             "QHeaderView::section { background-color: %1; color: %2; padding: 5px; border: none; border-radius: 10px; margin: 1px; %8 }"
                             "QTableWidget { alternate-background-color: %3; background-color: %4; gridline-color: %5; border: none solid %6; color: %7; %9 }"
                             "QTableWidget::item { padding: 4px; border-right: none; border-bottom: none; }"
                             "QTableWidget::item:selected { background-color: %10; color: %11; }"
                             ).arg(colors.primaryColor, colors.headerTextColor, colors.tableAlternateBg, colors.tableBg,
                                  colors.tableGrid, colors.tableBorder, colors.textColor, headerFontStyle,
                                  baseFontStyle, colors.tableSelectedBg, colors.textColor);

    // Button style - flat buttons with no background
    QString buttonStyle = QString(
                              "QPushButton { "
                              "background-color: transparent; " // No background
                              "color: %1; " // Text color will change based on theme
                              "border: none; " // No border by default
                              "padding: 8px; "
                              "font-weight: bold; "
                              "font-size: 15px; " // Match table header size
                              "}"
                              "QPushButton:hover { "
                              "border: 1px solid %2; " // Border shows on hover with theme color
                              "}"
                              "QPushButton:pressed { "
                              "padding-top: 9px; "
                              "padding-bottom: 7px; "
                              "}"
                              ).arg(
                                  // First arg: text color (black for blue/green themes, white for dark)
                                  (theme == " 다크모드") ? "#E0E0E0" : "#333333",
                                  // Second arg: border color on hover (use primary theme color)
                                  colors.primaryColor
                                  );




    // 다크모드에서는 다른 아이콘 사용
    QString dropdownIcon = (theme == " 다크모드") ? ":/icons/dropdown_dark.png" : ":/icons/dropdown.png";

    QString comboStyle = QString(
                             "QComboBox { border: 1px solid %1; border-radius: 4px; padding: 4px; background-color: %2; color: %3; %5 }"
                             "QComboBox::drop-down { border: 0px; width: 20px; }"
                             "QComboBox::down-arrow { image: url(%4); width: 12px; height: 12px; }"
                             "QComboBox QAbstractItemView { border: 1px solid %1; selection-background-color: %6; color: %3; %5 }"
                             ).arg(colors.comboBorder, colors.comboBg, colors.textColor, dropdownIcon,
                                  baseFontStyle, colors.primaryColor);

    QString dialogStyleBase = QString("QDialog { background-color: %1; %2 }")
                                  .arg(colors.bgColorLight, baseFontStyle);

    QString dialogLabelStyle = QString("QDialog QLabel { color: %1; %2 }")
                                   .arg(colors.textColor, baseFontStyle);

    QString dialogInputStyle = QString(
                                   "QDialog QLineEdit, QDialog QSpinBox, QDialog QComboBox { padding: 5px; border-radius: 4px; "
                                   "border: 1px solid %1; background-color: %2; color: %3; %4 }"
                                   ).arg(colors.comboBorder, colors.inputBg, colors.inputTextColor, baseFontStyle);

    // 스타일 적용
    this->setStyleSheet(windowStyle);
    tableWidget->setStyleSheet(tableStyle);
    tableWidget->setAlternatingRowColors(true);

    // 버튼 스타일 적용
    addButton->setStyleSheet(buttonStyle);
    editButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    displayButton->setStyleSheet(buttonStyle);
    searchButton->setStyleSheet(buttonStyle);

    // 콤보박스 스타일 적용
    themeCombo->setStyleSheet(comboStyle);

    // 다이얼로그 스타일 적용
    QString globalDialogStyle = dialogStyleBase + dialogLabelStyle + dialogInputStyle;
    static_cast<QApplication*>(QApplication::instance())->setStyleSheet(globalDialogStyle);
}

// 소멸자
CustomerWidget::~CustomerWidget()
{
}
