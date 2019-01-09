#include <QApplication>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPainter>
#include <QFontDatabase>

void printForm(bool pdf);

QString g_fontFamily = "";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int id = QFontDatabase::addApplicationFont(":/IBMPlexMono-Medium.ttf");
    g_fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);

    printForm(true); //pdf
    printForm(false); //physical printer

    return a.exec();
}


void printForm(bool pdf)
{
    //setup printer
    QPrinter *printer;
    if(pdf)
    {
        printer = new QPrinter(QPrinter::ScreenResolution);
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName("./regular.pdf");
    }
    else
    {
        printer = new QPrinter(QPrinterInfo::defaultPrinter(), QPrinter::ScreenResolution);
        printer->setOutputFormat(QPrinter::NativeFormat);
    }
    printer->setOrientation(QPrinter::Orientation::Landscape);
    QPageSize customSize(QSizeF(105.0, 157.76), QPageSize::Millimeter, "", QPageSize::ExactMatch);
    printer->setPageSize(customSize);

    //draw background
    QImage img(":/regular_transfer_form");
    QPainter painter(printer);
    painter.drawImage(QRect(0,0, printer->width(), printer->height()), img, QRect(0,0, img.width(), img.height()));

    //draw text
    painter.setPen(Qt::black);
    QFont font(g_fontFamily, 11);
    font.setStyleHint(QFont::TypeWriter);
    font.setLetterSpacing(QFont::PercentageSpacing, 212);

    painter.setFont(font);

    QTextOption textOption(Qt::AlignVCenter | Qt::AlignLeft);
    textOption.setWrapMode(QTextOption::NoWrap);

    QString text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit";

    QRectF relativeArea(0.05838, 0.0783, 0.8971, 0.04583); //coords relative to background

    QRectF areaOnPaper(relativeArea.x() * printer->width(),
                                relativeArea.y() * printer->height(),
                                relativeArea.width() * printer->width(),
                                relativeArea.height() * printer->height()
                                );

    painter.drawText(areaOnPaper, text, textOption); //draw 1-st line wit percentage Spacing

    //set spacing to absolute
    font.setLetterSpacing(QFont::AbsoluteSpacing, 10);
    painter.setFont(font);

    areaOnPaper.setRect(areaOnPaper.x(), areaOnPaper.y() + 28, areaOnPaper.width(), areaOnPaper.height());
    painter.drawText(areaOnPaper, text, textOption); //draw 2-nd line wit absolute Spacing

}
