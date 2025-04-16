#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 폰트 로드
    int fontId = QFontDatabase::addApplicationFont(":/fonts/R.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);

    // 폰트 설정
    QFont defaultFont(fontFamily,10);
    QApplication::setFont(defaultFont);

    // 위젯 생성 및 표시
    CustomerWidget w;
    w.show();

    return a.exec();
}
