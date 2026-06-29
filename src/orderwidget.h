#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>

class QTableWidget;
class QPushButton;

class OrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderWidget(QWidget *parent = nullptr);

    void addOrder(const QString &localOrderId,
                  const QString &exchangeOrderId,
                  const QString &status,
                  const QString &orderTime,
                  double volume,
                  double price);

private slots:
    void onCancelClicked();

private:
    QTableWidget *m_table;
};

#endif // ORDERWIDGET_H