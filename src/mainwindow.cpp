#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTreeWidget>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hello World");
    resize(400, 300);

    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    m_label = new QLabel("Hello, World!", this);
    m_label->setAlignment(Qt::AlignCenter);

    QFont font = m_label->font();
    font.setPointSize(24);
    m_label->setFont(font);

    layout->addWidget(m_label);
    setCentralWidget(centralWidget);

    // Left navigation tree, docked like a trade-station side panel.
    auto *navDock = new QDockWidget(QStringLiteral("导航"), this);
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
    
}
