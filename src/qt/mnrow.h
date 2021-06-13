#ifndef MYADDRESSROW_H
#define MYADDRESSROW_H

#include <QWidget>

namespace Ui
{
class MNRow;
}

class MNRow : public QWidget
{
    Q_OBJECT

public:
    explicit MNRow(QWidget* parent = nullptr);
    ~MNRow();

    void updateView(QString address, QString label);

private:
    Ui::MNRow* ui;
};

#endif // MYADDRESSROW_H