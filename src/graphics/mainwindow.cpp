#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_simulationTimer = new QTimer(this);
    connect(m_simulationTimer,&QTimer::timeout,this,&MainWindow::onSimulateIteration);
    m_simulationTimeMS = 100;

   /* QFrame* MainFrame = new QFrame(this);
    MainFrame->setWindowTitle("Qt SFML");
    MainFrame->resize(400, 400);
    MainFrame->show();*/
    // Create a SFML view inside the main frame
    SFMLView = new Canvas(this, QPoint(20, 150), QSize(500, 250)/*,&m_shapes*/);
    connect(SFMLView,&Canvas::checkKeyEvents,this,&MainWindow::checkKeyEvents);
    SFMLView->show();

    m_tool_move = new Tool(Tool::Type::moveModifier);
    m_tool_connect = new Tool(Tool::Type::signalConnector);
    m_tool_disconnect = new Tool(Tool::Type::signalRemover);

    m_grid = new Grid();

    LogicGate_CONST *cGate1 = new LogicGate_CONST(Vector2i(100,80));
    cGate1->voltage(5);
    LogicGate_CONST *cGate2 = new LogicGate_CONST(Vector2i(100,120));
    cGate2->voltage(5);
    LogicGate_CONST *cGate3 = new LogicGate_CONST(Vector2i(100,160));
    cGate3->voltage(5);

    m_shapes.push_back(cGate1);
    m_shapes.push_back(cGate2);
    m_shapes.push_back(cGate3);
    m_shapes.push_back(new LogicGate_AND(Vector2i(200,80),2));
    m_shapes.push_back(new LogicGate_AND(Vector2i(300,80),2));
    m_shapes.push_back(new LogicGate_AND(Vector2i(100,80),2));
    m_shapes.push_back(new LogicGate_AND(Vector2i(400,80),3));
    //m_shapes.push_back(new Gate(Vector2i(200,80)));
    //m_shapes.push_back(new Gate());


    setupRibbon();

    BlockParser parser;
    parser.readFile("..\\..\\Blocks\\AND.verilog");

}

MainWindow::~MainWindow()
{
    delete SFMLView;
    delete ui;
}

/*void MainWindow::onToolMoveButtonAction(QAction *action)
{
    if(action->Trigger)
    {
        qDebug() << "button pressed";
    }
    qDebug() << "button pressed";

}*/
/*void MainWindow::resetToolButtonState()
{
   // m_moveToolButton->setDown(false);
   // m_connectToolButton->setDown(false);
}*/
void MainWindow::setupRibbon()
{
    // Hide ribbon dock title bar
      ui->ribbonDockWidget->setTitleBarWidget(new QWidget());

      // Add tabs to ribbon
      ui->ribbonTabWidget->addTab(QIcon(":/icons/briefcase_1.svg"), "Project");
      ui->ribbonTabWidget->addTab(QIcon(":/icons/monitor_1.svg"), "View");
      ui->ribbonTabWidget->addTab(QIcon(":/icons/engineering_1.svg"), "Tools");
      ui->ribbonTabWidget->addTab(QIcon(":/icons/information_1.svg"), "Help");

      // Add 'Open project' button
      QToolButton *openProjectButton = new QToolButton;
      openProjectButton->setText(tr("Open"));
      openProjectButton->setToolTip(tr("Open existing project"));
      openProjectButton->setIcon(QIcon(":/icons/live_folder_2.svg"));
      openProjectButton->setEnabled(true);
      ui->ribbonTabWidget->addButton("Project", "Project", openProjectButton);

      // Add 'New project' button
      QToolButton *newProjectButton = new QToolButton;
      newProjectButton->setText(tr("New"));
      newProjectButton->setToolTip(tr("Create new project"));
      newProjectButton->setIcon(QIcon(":/icons/create_new_2.svg"));
      newProjectButton->setEnabled(true);
      ui->ribbonTabWidget->addButton("Project", "Project", newProjectButton);

      // Add 'Save project' button
      QToolButton *saveProjectButton = new QToolButton;
      saveProjectButton->setText(tr("Save"));
      saveProjectButton->setToolTip(tr("Save project"));
      saveProjectButton->setIcon(QIcon(":/icons/save_2.svg"));
      saveProjectButton->setEnabled(false);
      ui->ribbonTabWidget->addButton("Project", "Project", saveProjectButton);

      // Add 'Open file' button
      QToolButton *openFileButton = new QToolButton(this);
      openFileButton->setText(tr("File"));
      openFileButton->setToolTip(tr("Open file or directory"));
      openFileButton->setIcon(QIcon(":/icons/add_folder_2.svg"));

      // Add dropdown menu to button
      openFileButton->setPopupMode(QToolButton::MenuButtonPopup);
      QMenu *menu = new QMenu("Title");
      //menu->addAction(QIcon(":/icons/folder_2.svg"),
      //                "Recent directory");
      menu->addAction(QIcon(":/icons/file_2.svg"), "Recent file 1");
      menu->addAction(QIcon(":/icons/file_2.svg"), "Recent file 2");
      menu->addAction(QIcon(":/icons/file_2.svg"), "Recent file 3");
      openFileButton->setMenu(menu);

      ui->ribbonTabWidget->addButton("Project", "Import", openFileButton);

      // Add 'Open database' button
      QToolButton *openDatabaseButton = new QToolButton;
      openDatabaseButton->setText(tr("Database"));
      openDatabaseButton->setToolTip(tr("Connect to database"));
      openDatabaseButton->setIcon(QIcon(":/icons/add_database_2.svg"));
      ui->ribbonTabWidget->addButton("Project", "Import", openDatabaseButton);


      // Add 'Connect to web service' button
      QToolButton *connectWebserviceButton = new QToolButton;
      connectWebserviceButton->setText(tr("Web service"));
      connectWebserviceButton->setToolTip(tr("Connect to web service"));
      connectWebserviceButton->setIcon(QIcon(":/icons/add_link_2.svg"));

      ui->ribbonTabWidget->addButton("Project", "Import", connectWebserviceButton);

      //View
      {
          // Add 'Move tool' button
          QToolButton *m_viewSimulateButton = new QToolButton;
          m_viewSimulateButton->setText(tr("Play"));
          m_viewSimulateButton->setToolTip(tr("Simulate Circuit"));
          m_viewSimulateButton->setIcon(QIcon(":/icons/play.svg"));
          m_viewSimulateButton->setEnabled(true);
          ui->ribbonTabWidget->addButton("View", "Simulation", m_viewSimulateButton);
          connect(m_viewSimulateButton , &QAbstractButton::pressed, this, &MainWindow::onViewSimulate);
      }

      // Tools
      {
          // Add 'Move tool' button
          m_moveToolButton = new QToolButton;
          m_moveToolButton->setText(tr("Move"));
          m_moveToolButton->setToolTip(tr("Move Components"));
          m_moveToolButton->setIcon(QIcon(":/icons/move.svg"));
          m_moveToolButton->setEnabled(true);
          ui->ribbonTabWidget->addButton("Tools", "Component", m_moveToolButton);
          connect(m_moveToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolMoveButtonPressed);

          // Add 'Connect tool' button
          m_connectToolButton = new QToolButton;
          m_connectToolButton->setText(tr("Connect"));
          m_connectToolButton->setToolTip(tr("Connect Components"));
          m_connectToolButton->setIcon(QIcon(":/icons/link.svg"));
          m_connectToolButton->setEnabled(true);
        //  m_connectToolButton->setCheckable(true);
          ui->ribbonTabWidget->addButton("Tools", "Component", m_connectToolButton);
          connect(m_connectToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolConnectButtonPressed);

          // Add 'Disconnect tool' button
          m_disconnectToolButton = new QToolButton;
          m_disconnectToolButton->setText(tr("Disconnect"));
          m_disconnectToolButton->setToolTip(tr("Disconnect Components"));
          m_disconnectToolButton->setIcon(QIcon(":/icons/disconnect.svg"));
          m_disconnectToolButton->setEnabled(true);
        //  m_disconnectToolButton->setCheckable(true);
          ui->ribbonTabWidget->addButton("Tools", "Component", m_disconnectToolButton);
          connect(m_disconnectToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolDisconnectButtonPressed);

      }

}
void MainWindow::onViewSimulate()
{
    Physics::displayPhysical = !Physics::displayPhysical;
    if(Physics::displayPhysical)
        m_simulationTimer->start(m_simulationTimeMS);
    else
        m_simulationTimer->stop();
}
void MainWindow::onToolMoveButtonPressed()
{

    qDebug() << "onToolMoveButtonPressed";
    //resetToolButtonState();
    if(Tool::getSelectedType() == Tool::Type::moveModifier)
    {
       // resetToolButtonState();
        Tool::unselect();
       // m_moveToolButton->setDown(false);
       // m_moveToolButton->setCheckable(false);
    }
    else
    {
      //  m_connectToolButton->setDown(false);
        m_tool_move->setSelected(true);
       // m_moveToolButton->setCheckable(true);
       // m_moveToolButton->setDown(true);
    }

}
void MainWindow::onToolConnectButtonPressed()
{
    qDebug() << "onToolConnectButtonPressed";
    //resetToolButtonState();
    if(Tool::getSelectedType() == Tool::Type::signalConnector)
    {
       // resetToolButtonState();
        Tool::unselect();
       // m_connectToolButton->setDown(false);
       // m_connectToolButton->setCheckable(false);
    }
    else
    {
       // m_moveToolButton->setDown(false);
        m_tool_connect->setSelected(true);
        //m_connectToolButton->setCheckable(true);
       //m_connectToolButton->setDown(true);
    }
}
void MainWindow::onToolDisconnectButtonPressed()
{
    //resetToolButtonState();
    if(Tool::getSelectedType() == Tool::Type::signalRemover)
    {
        //resetToolButtonState();
        Tool::unselect();
    }
    else
    {

        m_tool_disconnect->setSelected(true);

    }
}


void MainWindow::resizeEvent(QResizeEvent* e)
{
    QSize size(this->geometry().width()-40,this->geometry().height()-180);
    SFMLView->setSize(size);
}
void MainWindow::checkKeyEvents()
{
    //if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
    {
        ui->ribbonTabWidget->setCurrentIndex(2);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M) &&
       Tool::getSelectedType() != Tool::Type::moveModifier)
    {
        onToolMoveButtonPressed();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::C) &&
       Tool::getSelectedType() != Tool::Type::signalConnector)
    {
        onToolConnectButtonPressed();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X) &&
       Tool::getSelectedType() != Tool::Type::signalRemover)
    {
        onToolDisconnectButtonPressed();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
       // resetToolButtonState();
        Tool::unselect();
        //m_moveToolButton->setDown(false);
        //m_connectToolButton->setDown(false);
        //m_connectToolButton->setCheckable(false);
        //m_moveToolButton->setCheckable(false);
    }
}

void MainWindow::onSimulateIteration()
{
    Gate::global_processLogic();
}
