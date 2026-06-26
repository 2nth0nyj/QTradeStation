#ifndef TRADINGCARDWIDGET_H
#define TRADINGCARDWIDGET_H

#include <QFrame>

class QLabel;
class QPushButton;

class TradingCardWidget : public QFrame
{
    Q_OBJECT

public:
    explicit TradingCardWidget(const QString &exchange,
                               const QString &symbol,
                               double price,
                               QWidget *parent = nullptr);

signals:
    void buyClicked(const QString &exchange, const QString &symbol);
    void sellClicked(const QString &exchange, const QString &symbol);

private:
    QLabel *m_exchangeLabel;
    QLabel *m_symbolLabel;
    QLabel *m_priceLabel;
    QPushButton *m_buyButton;
    QPushButton *m_sellButton;
};

#endif // TRADINGCARDWIDGET_H
