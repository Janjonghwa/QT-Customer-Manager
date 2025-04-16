#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H

#include "model.h"

// Qt 기본 위젯
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QApplication>
#include <QFont>
#include <QWidget>
#include <QIcon>

// Qt 레이아웃
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

// Qt 다이얼로그 관련
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

// Qt 테이블 관련
#include <QHeaderView>
#include <QTableWidgetItem>

// Qt 그래픽 효과
#include <QGraphicsDropShadowEffect>

// 프로그램 종료 시 데이터 저장
#include <QCloseEvent>

class CustomerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerWidget(QWidget *parent = nullptr);
    ~CustomerWidget();

private slots:
    void addCustomer();
    void deleteCustomer();
    void displayAllCustomers();
    void editCustomer();
    void openSearchDialog();
    void sortTable(int column);
    void onTableDoubleClicked(const QModelIndex &index);

private:
    CustomerModel model;

    // UI 변수 생성
    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *displayButton;
    QPushButton *editButton;
    QPushButton *searchButton;
    QComboBox *themeCombo;
    QLabel *countLabel;

    // 정렬 관련 변수
    int sortColumn;
    bool sortOrder; // true: 오름차순, false: 내림차순

    // 초기 화면 세팅 (생성자에서 한번만 불려짐)
    void setupUI();
    // 테이블 세팅 함수 setupUI에서 최초 한번만 불려짐
    void setupTable(QVBoxLayout *mainLayout);
    // 버튼 세팅 함수 setupUI에서 최초 한번만 불려짐
    void setupButtons(QVBoxLayout *mainLayout);
    // 버튼 생성 함수 setupButtons에서만 호출됨
    QPushButton* createButton(const QString &text, void (CustomerWidget::*slot)());

    // 테이블 업데이트 함수
    void updateTable();
    // 테이블에 고객 명단 작성하는 함수(한명씩)
    void addCustomerToTable(int row, const Customer& customer);
    // 테이블 헤더 세팅
    void setupTableColumns();

    // 고객 정보 다이얼로그
    bool showCustomerDialog(const QString& title, Customer& customer);

    // 정렬 및 검색 함수
    static bool compareCustomers(const Customer& a, const Customer& b, int column, bool ascending);
    void performSearch(const QString& searchField, const QString& searchText);

    // 테마 변경 함수
    void changeTheme(const QString &theme);
    // 테마 적용 함수
    void applyTheme(const QString &theme);

private:
    // 파일 경로 상수
    const QString DATA_FILE_PATH = "customers.json";

    // 저장 및 불러오기 함수
    void saveCustomerData();
    void loadCustomerData();

    // 프로그램 종료 이벤트 처리
    void closeEvent(QCloseEvent *event) override;
};


#endif // CUSTOMERWIDGET_H
