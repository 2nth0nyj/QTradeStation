#include "orderwidget.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLocale>
#include <QDateTime>
#include <QPushButton>

OrderWidget::OrderWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);

    m_table = new QTableWidget(0, 8, this);
    m_table->setHorizontalHeaderLabels({
        QStringLiteral("Local Order ID"),
        QStringLiteral("Exchange Order ID"),
        QStringLiteral("Status"),
        QStringLiteral("Order Time"),
        QStringLiteral("Volume"),
        QStringLiteral("Price"),
        QStringLiteral("Total"),
        QStringLiteral("Action")
    });

    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);

    // Columns resize to fit their longest content
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    layout->addWidget(m_table);

    // Populate with sample orders
    addOrder(QStringLiteral("LOC-20260630-0001"),
             QStringLiteral("BIN-ORD-8821345"),
             QStringLiteral("Filled"),
             QStringLiteral("2026-06-30 09:35:12"),
             150.0,
             425.30);

    addOrder(QStringLiteral("LOC-20260630-0002"),
             QStringLiteral("FUT-ORD-7712098"),
             QStringLiteral("Open"),
             QStringLiteral("2026-06-30 09:42:05"),
             500.0,
             185.20);

    addOrder(QStringLiteral("LOC-20260630-0003"),
             QStringLiteral("IB-ORD-5543210"),
             QStringLiteral("Partial Filled"),
             QStringLiteral("2026-06-30 10:01:33"),
             300.0,
             245.60);

    addOrder(QStringLiteral("LOC-20260630-0004"),
             QStringLiteral("BIN-ORD-9987612"),
             QStringLiteral("Cancelled"),
             QStringLiteral("2026-06-30 10:15:47"),
             1000.0,
             880.10);

    addOrder(QStringLiteral("LOC-20260630-0005"),
             QStringLiteral("FUT-ORD-3341098"),
             QStringLiteral("Closed"),
             QStringLiteral("2026-06-30 10:28:19"),
             200.0,
             210.45);

    addOrder(QStringLiteral("LOC-20260630-0006"),
             QStringLiteral("IB-ORD-1209876"),
             QStringLiteral("Filled"),
             QStringLiteral("2026-06-30 11:05:02"),
             80.0,
             505.60);

    addOrder(QStringLiteral("LOC-20260630-0007"),
             QStringLiteral("BIN-ORD-5567341"),
             QStringLiteral("Open"),
             QStringLiteral("2026-06-30 11:22:51"),
             600.0,
             178.90);

    addOrder(QStringLiteral("LOC-20260630-0008"),
             QStringLiteral("FUT-ORD-4456782"),
             QStringLiteral("Partial Filled"),
             QStringLiteral("2026-06-30 13:10:08"),
             250.0,
             675.20);
}

void OrderWidget::addOrder(const QString &localOrderId,
                           const QString &exchangeOrderId,
                           const QString &status,
                           const QString &orderTime,
                           double volume,
                           double price)
{
    const int row = m_table->rowCount();
    m_table->insertRow(row);

    double total = volume * price;
    QLocale locale = QLocale::system();

    auto setItem = [&](int col, const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setTextAlignment(col >= 4 ? Qt::AlignRight | Qt::AlignVCenter
                                        : Qt::AlignLeft | Qt::AlignVCenter);
        m_table->setItem(row, col, item);
    };

    setItem(0, localOrderId);
    setItem(1, exchangeOrderId);
    setItem(2, status);
    setItem(3, orderTime);
    setItem(4, locale.toString(volume, 'f', 2));
    setItem(5, locale.toString(price, 'f', 2));
    setItem(6, locale.toString(total, 'f', 2));

    // Action column: Cancel button for open orders, placeholder otherwise
    if (status == QStringLiteral("Open") || status == QStringLiteral("Partial Filled")) {
        auto *cancelBtn = new QPushButton(QStringLiteral("Cancel"), m_table);
        cancelBtn->setProperty("row", row);
        cancelBtn->setStyleSheet(
            QStringLiteral("QPushButton {"
                           "  background-color: #e74c3c;"
                           "  color: white;"
                           "  border: none;"
                           "  border-radius: 3px;"
                           "  padding: 4px 12px;"
                           "  font-weight: bold;"
                           "}"
                           "QPushButton:hover {"
                           "  background-color: #c0392b;"
                           "}"
                           "QPushButton:pressed {"
                           "  background-color: #a93226;"
                           "}"));
        connect(cancelBtn, &QPushButton::clicked,
                this, &OrderWidget::onCancelClicked);
        m_table->setCellWidget(row, 7, cancelBtn);
    } else {
        setItem(7, QStringLiteral("—"));
    }
}

void OrderWidget::onCancelClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    bool ok = false;
    int row = btn->property("row").toInt(&ok);
    if (!ok)
        return;

    // Update status to Cancelled
    auto *statusItem = new QTableWidgetItem(QStringLiteral("Cancelled"));
    statusItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_table->setItem(row, 2, statusItem);

    // Replace the Cancel button with a plain "—" text
    m_table->removeCellWidget(row, 7);
    auto *placeholder = new QTableWidgetItem(QStringLiteral("—"));
    placeholder->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_table->setItem(row, 7, placeholder);
}
