#include "mainwindow.h"
#include "dashboardwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QStyle>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("QTradeStation");
    resize(900, 600);

    // Central area with stacked widget to switch between views
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Left navigation tree, docked like a trade-station side panel.
    auto *navDock = new QDockWidget(QStringLiteral("Navigation"), this);
    navDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    navDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    navDock->setObjectName(QStringLiteral("NavDock"));

    m_tree = new QTreeWidget(navDock);
    m_tree->setHeaderHidden(true);
    m_tree->setUniformRowHeights(true);
    m_tree->setAnimated(true);
    navDock->setWidget(m_tree);
    addDockWidget(Qt::LeftDockWidgetArea, navDock);

    buildTree();
    setupDashboard();
    setupPlaceholderPages();

    // Connect tree item clicks
    connect(m_tree, &QTreeWidget::itemClicked,
            this, &MainWindow::onTreeItemClicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::buildTree()
{
    const QIcon folderIcon = style()->standardIcon(QStyle::SP_DirIcon);

    auto addFolder = [&](QTreeWidgetItem *parent, const QString &text) {
        auto *item = new QTreeWidgetItem(parent);
        item->setText(0, text);
        item->setIcon(0, folderIcon);
        return item;
    };

    // Top-level nodes.
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Dashboard"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Orders"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Positions"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Markets"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("News"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Portfolio Analytics"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Accounts"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Settings"));
    addFolder(m_tree->invisibleRootItem(), QStringLiteral("Help"));

    // Select "Dashboard" by default so the tree highlights the active page.
    m_tree->setCurrentItem(m_tree->topLevelItem(0));
}

void MainWindow::setupDashboard()
{
    m_dashboard = new DashboardWidget(this);

    // Add some sample trading cards
    m_dashboard->addCard("NYSE",      "AAPL",  210.45);
    m_dashboard->addCard("NASDAQ",    "GOOGL", 185.20);
    m_dashboard->addCard("NASDAQ",    "MSFT",  425.30);
    m_dashboard->addCard("NYSE",      "JPM",   198.75);
    m_dashboard->addCard("NYSE",      "TSLA",  245.60);
    m_dashboard->addCard("NASDAQ",    "AMZN",  178.90);
    m_dashboard->addCard("NYSE",      "BAC",   39.55);
    m_dashboard->addCard("NASDAQ",    "NVDA",  880.10);
    m_dashboard->addCard("NYSE",      "WMT",   172.30);

    m_stackedWidget->addWidget(m_dashboard);
    m_stackedWidget->setCurrentWidget(m_dashboard);
}

void MainWindow::setupPlaceholderPages()
{
    // List of tree item names (excluding "Dashboard" which has its own widget)
    const QStringList pageNames = {
        QStringLiteral("Orders"),
        QStringLiteral("Positions"),
        QStringLiteral("Markets"),
        QStringLiteral("News"),
        QStringLiteral("Portfolio Analytics"),
        QStringLiteral("Accounts"),
        QStringLiteral("Settings"),
        QStringLiteral("Help")
    };

    for (const QString &name : pageNames) {
        auto *page = new QWidget(this);
        auto *layout = new QVBoxLayout(page);
        auto *label = new QLabel(
            QStringLiteral("%1 - Coming Soon").arg(name), page);
        QFont font = label->font();
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        m_stackedWidget->addWidget(page);
        m_placeholderPages.append(page);
    }
}

void MainWindow::onTreeItemClicked(QTreeWidgetItem *item, int /*column*/)
{
    const QString text = item->text(0);

    if (text == QStringLiteral("Dashboard")) {
        m_stackedWidget->setCurrentWidget(m_dashboard);
        return;
    }

    // Map tree item names to placeholder page indices
    // The placeholder pages are created in the same order as the tree items
    // (excluding Dashboard which is at index 0)
    const QStringList pageNames = {
        QStringLiteral("Orders"),
        QStringLiteral("Positions"),
        QStringLiteral("Markets"),
        QStringLiteral("News"),
        QStringLiteral("Portfolio Analytics"),
        QStringLiteral("Accounts"),
        QStringLiteral("Settings"),
        QStringLiteral("Help")
    };

    int index = pageNames.indexOf(text);
    if (index >= 0 && index < m_placeholderPages.size()) {
        m_stackedWidget->setCurrentWidget(m_placeholderPages.at(index));
    }
}
