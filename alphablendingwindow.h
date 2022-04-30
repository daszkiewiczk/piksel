#ifndef ALPHABLENDINGWINDOW_H
#define ALPHABLENDINGWINDOW_H

#include <QDialog>

namespace Ui {
class AlphaBlendingWindow;
}

class AlphaBlendingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AlphaBlendingWindow(QWidget *parent = nullptr);
    ~AlphaBlendingWindow();
    QImage im;
protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::AlphaBlendingWindow *ui;
};

#endif // ALPHABLENDINGWINDOW_H
