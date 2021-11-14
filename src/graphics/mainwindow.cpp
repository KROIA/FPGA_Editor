#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_simulationTimer = new QTimer(this);
    connect(m_simulationTimer,&QTimer::timeout,this,&MainWindow::onSimulateIteration);
    m_simulationTimeMS = 5;
    m_makeCopyOf = nullptr;

    // Create a SFML view inside the main frame
    SFMLView = new Canvas(this, QPoint(20, 150), QSize(500, 250));
    connect(&frameUpdateTimer,&QTimer::timeout,this,&MainWindow::onFrameUpdate);
    connect(SFMLView,&Canvas::checkKeyEvents,this,&MainWindow::checkKeyEvents);
    SFMLView->show();
    frameUpdateTimer.start(20);

    m_tool_move = new Tool(Tool::Type::moveModifier);
    m_tool_connect = new Tool(Tool::Type::signalConnector);
    m_tool_disconnect = new Tool(Tool::Type::signalRemover);
    m_tool_moduleRemover = new Tool(Tool::Type::moduleRemover);

    m_grid = new Grid();
    SFMLView->addShape(0,m_grid);

    m_topModule = new Module();
    m_topModule->setViewInside(true);
    m_topModule->addIO("input1",Pin::Direction::input);
    //m_topModule->addIO("input2",Pin::Direction::input);
    //m_topModule->addIO("input3",Pin::Direction::input);
    m_topModule->addIO("output1",Pin::Direction::output);
    //m_topModule->addIO("output1",Pin::Direction::output);
    m_openModuleStack.push_back(m_topModule);

    Module *newModule = new Module();
    newModule->name("test");
    newModule->addIO("in1",Pin::Direction::input);
    newModule->addIO("in2",Pin::Direction::input);
    newModule->addIO("out",Pin::Direction::output);
    newModule->setPos(Vector2i(50,50));
    m_topModule->addGate(newModule);
    connect(newModule,&Module::insideViewEnter,this,&MainWindow::onInsideViewEnter);
    connect(newModule,&Module::insideViewExit,this,&MainWindow::onInsideViewExit);
    //newModule->setViewInside(true);


    setupRibbon();
    resizeEvent(nullptr);

}

MainWindow::~MainWindow()
{
    delete SFMLView;
    delete ui;
}


void MainWindow::setupRibbon()
{
    // Hide ribbon dock title bar
      ui->ribbonDockWidget->setTitleBarWidget(new QWidget());

      // Add tabs to ribbon
      ui->ribbonTabWidget->addTab(QIcon(":/Resources/icons/briefcase_1.svg"), "Project");
      ui->ribbonTabWidget->addTab(QIcon(":/Resources/icons/monitor_1.svg"), "View");
      ui->ribbonTabWidget->addTab(QIcon(":/Resources/icons/engineering_1.svg"), "Edit");
      ui->ribbonTabWidget->addTab(QIcon(":/Resources/icons/information_1.svg"), "Help");

      /*// Add 'Open project' button
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
      */

      //View
      {
          // Add 'Move tool' button
        /*  QToolButton *m_viewSimulateButton = new QToolButton;
          m_viewSimulateButton->setText(tr("Play"));
          m_viewSimulateButton->setToolTip(tr("Simulate Circuit"));
          m_viewSimulateButton->setIcon(QIcon(":/icons/play.svg"));
          m_viewSimulateButton->setEnabled(true);
          ui->ribbonTabWidget->addButton("View", "Simulation", m_viewSimulateButton);
          connect(m_viewSimulateButton , &QAbstractButton::pressed, this, &MainWindow::onViewSimulate);*/
          m_moveToolButton = addRibbonButton("View","Simulation","Play","play.svg",
                                             "Simulate circuit",&MainWindow::onViewSimulate);
      }

      // Edit
      {
          // Add 'Move tool' button
        /*  m_moveToolButton = new QToolButton;
          m_moveToolButton->setText(tr("Move"));
          m_moveToolButton->setToolTip(tr("Move Components"));
          m_moveToolButton->setIcon(QIcon(":/icons/move.svg"));
          m_moveToolButton->setEnabled(true);
          ui->ribbonTabWidget->addButton("Edit", "Component", m_moveToolButton);
          connect(m_moveToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolMoveButtonPressed);*/
          m_moveToolButton = addRibbonButton("Edit","Component","Move","move.svg",
                                             "Move components",&MainWindow::onToolMoveButtonPressed);

          // Add 'Connect tool' button
         /* m_connectToolButton = new QToolButton;
          m_connectToolButton->setText(tr("Connect"));
          m_connectToolButton->setToolTip(tr("Connect Components"));
          m_connectToolButton->setIcon(QIcon(":/icons/link.svg"));
          m_connectToolButton->setEnabled(true);
        //  m_connectToolButton->setCheckable(true);
          ui->ribbonTabWidget->addButton("Edit", "Component", m_connectToolButton);
          connect(m_connectToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolConnectButtonPressed);*/
          m_connectToolButton = addRibbonButton("Edit","Component","Connect","link.svg",
                                                "Connect components",&MainWindow::onToolConnectButtonPressed);

          // Add 'Disconnect tool' button
         /* m_disconnectToolButton = new QToolButton;
          m_disconnectToolButton->setText(tr("Disconnect"));
          m_disconnectToolButton->setToolTip(tr("Disconnect Components"));
          m_disconnectToolButton->setIcon(QIcon(":/icons/disconnect.svg"));
          m_disconnectToolButton->setEnabled(true);
        //  m_disconnectToolButton->setCheckable(true);
          ui->ribbonTabWidget->addButton("Edit", "Component", m_disconnectToolButton);*/
          //connect(m_disconnectToolButton , &QAbstractButton::pressed, this, &MainWindow::onToolDisconnectButtonPressed);
          m_disconnectToolButton = addRibbonButton("Edit","Component","Disconnect","disconnect.svg",
                                                   "Disconnect components",&MainWindow::onToolDisconnectButtonPressed);

          m_deleteToolButton = addRibbonButton("Edit","Component","Delete","delete.svg",
                                                   "Delete components",&MainWindow::onToolDeleteButtonPressed);


      }
      // Module
      {
          addRibbonButton("Edit","Module","Add Input","moduleInput.svg",
                          "Adds a Input to the Module",&MainWindow::onToolAddIOInputButtonPressed);
          addRibbonButton("Edit","Module","Add Output","moduleOutput.svg",
                          "Adds a Output to the Module",&MainWindow::onToolAddIOOutputButtonPressed);
          addRibbonButton("Edit","Module","Exit Module","moduleExit.svg",
                          "Get out of this Module",&MainWindow::onToolMoveModuleUp);

      }
      // Add Standard Gates
      {
          string tab = "Edit";
          string group = "Add gate";
          string toolTipBegin = "Adds a ";
          string toolTipEnd = " gate";
          string svgBegin = "gates/gate_";
          string svgEnd = ".svg";
          string gateType;

          gateType = LogicGate::logicToString(LogicGate::Logic::AND);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateAND);
          gateType = LogicGate::logicToString(LogicGate::Logic::OR);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateOR);
          gateType = LogicGate::logicToString(LogicGate::Logic::XOR);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateXOR);
          gateType = LogicGate::logicToString(LogicGate::Logic::NAND);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateNAND);
          gateType = LogicGate::logicToString(LogicGate::Logic::NOR);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateNOR);
          gateType = LogicGate::logicToString(LogicGate::Logic::XNOR);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateXNOR);
          gateType = LogicGate::logicToString(LogicGate::Logic::NOT);
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddGateNOT);
          gateType = "CONST";
          addRibbonButton(tab,group,gateType,svgBegin+ gateType+svgEnd,
                          toolTipBegin+gateType+toolTipEnd,&MainWindow::onToolAddConst);


      }

}
QToolButton* MainWindow::addRibbonButton(const string &tab,
                             const string &group,
                             const string &name,
                             const string &logo,
                             const string &toolTip,
                             void (MainWindow::*slot)(void))
{
    QToolButton *button = new QToolButton;
    button->setText(tr(name.c_str()));
    button->setToolTip(tr(toolTip.c_str()));
    button->setIcon(QIcon(QString::fromStdString(":Resources/icons/"+logo)));
    button->setEnabled(true);
    ui->ribbonTabWidget->addButton(QString::fromStdString(tab), QString::fromStdString(group), button);
    connect(button , &QAbstractButton::pressed, this, slot);
    //button->setAutoRaise(false);

    return button;
}

void MainWindow::onFrameUpdate()
{
    for(size_t i=0; i<m_shapesDeleteLater.size(); i++)
    {
        for(size_t a=0; a<m_shapes.size(); a++)
        {
            if(m_shapes[a] == m_shapesDeleteLater[i])
            {
                delete m_shapes[a];
                m_shapes.erase(m_shapes.begin() + a);
                a--;
                goto nextDel;
            }
        }
        nextDel:;
    }
    m_shapesDeleteLater.clear();

    if(m_makeCopyOf != nullptr)
    {
        Gate *newGate = m_makeCopyOf->clone();
        addGate(newGate);
        m_tool_move->setSelected(true);
        Tool::getSelected()->isUsed(true);
        newGate->snapToMouse(true);
        unselectAllButtons();
        m_moveToolButton->setStyleSheet("background-color: #cadeea");

        m_makeCopyOf = nullptr;
    }


    for(size_t i=0; i<m_shapesAddLater.size(); i++)
    {
        //m_shapes.push_back(m_shapesAddLater[i]);
        m_openModuleStack[m_openModuleStack.size()-1]->addGate(m_shapesAddLater[i]);
        connect(m_shapesAddLater[i],&Gate::placed,this,&MainWindow::onPlaced);
        connect(m_shapesAddLater[i],&Gate::startsMoving,this,&MainWindow::onStartMoving);
        connect(m_shapesAddLater[i],&Gate::clicked,this,&MainWindow::onClicked);
        connect(m_shapesAddLater[i],&Gate::addCopyOf,this,&MainWindow::onAddCopyOf);
        //connect(m_shapesAddLater[i],&Shape::deleteRequest,this,&MainWindow::onDeleteRequest);
    }
    m_shapesAddLater.clear();

    if(!m_simulationTimer->isActive())
    {
        m_topModule->utilityUpdate();
    }

    SFMLView->clearShape(1);
    SFMLView->addShape(1,m_openModuleStack[m_openModuleStack.size()-1]);
    SFMLView->repaint();

}


void MainWindow::onViewSimulate()
{
    Physics::displayPhysical = !Physics::displayPhysical;
    if(Physics::displayPhysical)
        m_simulationTimer->start(m_simulationTimeMS);
    else
        m_simulationTimer->stop();
}

void MainWindow::unselectAllButtons()
{
    m_moveToolButton->setStyleSheet("outline:none;");
    m_connectToolButton->setStyleSheet("outline:none;");
    m_disconnectToolButton->setStyleSheet("outline:none;");
    m_deleteToolButton->setStyleSheet("outline:none;");
}
void MainWindow::onToolMoveButtonPressed()
{
    if(Tool::getSelectedType() == Tool::Type::moveModifier)
    {
        Tool::unselect();
        m_moveToolButton->setStyleSheet("outline:none;");
    }
    else
    {
        unselectAllButtons();
        m_moveToolButton->setStyleSheet("background-color: #cadeea");
        m_tool_move->setSelected(true);
    }
}
void MainWindow::onToolConnectButtonPressed()
{
    if(Tool::getSelectedType() == Tool::Type::signalConnector)
    {
        Tool::unselect();
        m_connectToolButton->setStyleSheet("outline:none;");
    }
    else
    {
        unselectAllButtons();
        m_connectToolButton->setStyleSheet("background-color: #cadeea");
        m_tool_connect->setSelected(true);
    }
}
void MainWindow::onToolDisconnectButtonPressed()
{
    if(Tool::getSelectedType() == Tool::Type::signalRemover)
    {
        Tool::unselect();
        m_disconnectToolButton->setStyleSheet("outline:none;");
    }
    else
    {
        unselectAllButtons();
        m_disconnectToolButton->setStyleSheet("background-color: #cadeea");
        m_tool_disconnect->setSelected(true);
    }
}
void MainWindow::onToolDeleteButtonPressed()
{
    if(Tool::getSelectedType() == Tool::Type::moduleRemover)
    {
        Tool::unselect();
        m_deleteToolButton->setStyleSheet("outline:none;");
    }
    else
    {
        unselectAllButtons();
        m_deleteToolButton->setStyleSheet("background-color: #cadeea");
        m_tool_moduleRemover->setSelected(true);
    }
}
void MainWindow::onToolAddIOInputButtonPressed()
{
    int index = 1;
    string name = "In_"+std::to_string(index);
    while(m_openModuleStack[m_openModuleStack.size()-1]->getPin(name) != nullptr)
    {
        index++;
        name = "In_"+std::to_string(index);
    }

    m_openModuleStack[m_openModuleStack.size()-1]->addIO(name,Pin::Direction::input);
}
void MainWindow::onToolAddIOOutputButtonPressed()
{
    int index = 1;
    string name = "Out_"+std::to_string(index);
    while(m_openModuleStack[m_openModuleStack.size()-1]->getPin(name) != nullptr)
    {
        index++;
        name = "Out_"+std::to_string(index);
    }
    m_openModuleStack[m_openModuleStack.size()-1]->addIO(name,Pin::Direction::output);
}
void MainWindow::onToolMoveModuleUp()
{
    m_openModuleStack[m_openModuleStack.size()-1]->setViewInside(false);
}

void MainWindow::onToolAddGateAND()
{
    addLogicGate(LogicGate::Logic::AND);
}
void MainWindow::onToolAddGateOR()
{
    addLogicGate(LogicGate::Logic::OR);
}
void MainWindow::onToolAddGateXOR()
{
    addLogicGate(LogicGate::Logic::XOR);
}
void MainWindow::onToolAddGateNAND()
{
    addLogicGate(LogicGate::Logic::NAND);
}
void MainWindow::onToolAddGateNOR()
{
    addLogicGate(LogicGate::Logic::NOR);
}
void MainWindow::onToolAddGateXNOR()
{
    addLogicGate(LogicGate::Logic::XNOR);
}
void MainWindow::onToolAddGateNOT()
{
    addLogicGate(LogicGate::Logic::NOT);
}
void MainWindow::onToolAddConst()
{

    if(Tool::getSelected() != nullptr && Tool::getSelected()->isUsed())
    {
        qDebug() << "Tool is in usage";
        return;
    }
    LogicGate_CONST *gConst = new LogicGate_CONST(Vector2i(100,80));
    gConst->voltage(Physics::logicLevelToVoltage(1));

    addGate(gConst);
    m_tool_move->setSelected(true);
    Tool::getSelected()->isUsed(true);
    gConst->snapToMouse(true);
    gConst->deleteOnEscape(true);
    unselectAllButtons();
    m_moveToolButton->setStyleSheet("background-color: #cadeea");
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    QRect geometry = this->geometry();
    QSize size(geometry.width()-40,geometry.height()-180);
    SFMLView->setSize(size);

    ui->ribbonDockWidget->setGeometry(QRect(5,1,geometry.width()-10,120));
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
        unselectAllButtons();
        //m_moveToolButton->setDown(false);
        //m_connectToolButton->setDown(false);
        //m_connectToolButton->setCheckable(false);
        //m_moveToolButton->setCheckable(false);
    }
}

void MainWindow::onSimulateIteration()
{
    //Gate::global_processLogic();
    m_topModule->utilityUpdate();
    m_topModule->readInputs();
    m_topModule->processLogic();
    m_topModule->setOutputs();
    /*for(size_t i=0; i<m_moduleList.size(); i++)
    {
        m_moduleList[i]->readInputs();
    }
    for(size_t i=0; i<m_moduleList.size(); i++)
    {
        m_moduleList[i]->processLogic();
    }
    for(size_t i=0; i<m_moduleList.size(); i++)
    {
        m_moduleList[i]->setOutputs();
    }
    */
}

void MainWindow::onDeleteRequest(Shape *shape)
{
    m_shapesDeleteLater.push_back(shape);
}

void MainWindow::onPlaced(Gate *gate)
{

}
void MainWindow::onStartMoving(Gate *gate)
{

}
void MainWindow::onClicked(Gate *gate)
{

}
void MainWindow::onAddCopyOf(Gate *gate)
{
    m_makeCopyOf = gate;
}
void MainWindow::onInsideViewEnter(Module *module)
{
    Tool::unselect();
    m_openModuleStack[m_openModuleStack.size()-1]->setVisibility(false);
    m_openModuleStack.push_back(module);
}
void MainWindow::onInsideViewExit(Module *module)
{
    if(module == m_topModule)
        return;
    for(size_t i=0; i<m_openModuleStack.size(); i++)
    {
        if(m_openModuleStack[i] == module)
        {
            m_openModuleStack.erase(m_openModuleStack.begin()+i,m_openModuleStack.end());
        }
    }
    m_openModuleStack[m_openModuleStack.size()-1]->setVisibility(true);
}

void MainWindow::addGate(Gate *gate)
{
    m_shapesAddLater.push_back(gate);
}

void MainWindow::addLogicGate(LogicGate::Logic logic)
{
    if(Tool::getSelected() != nullptr && Tool::getSelected()->isUsed())
    {
        qDebug() << "Tool is in usage";
        return;
    }
    LogicGate *gate = new LogicGate(Vector2i(0,0),2);
    if(gate != nullptr)
    {
        gate->logic(logic);

        m_tool_move->setSelected(true);
        Tool::getSelected()->isUsed(true);
        gate->snapToMouse(true);
        gate->deleteOnEscape(true);
        addGate(gate);
        unselectAllButtons();
        m_moveToolButton->setStyleSheet("background-color: #cadeea");
    }
    else
    {
        qDebug() << "can't create gate";
    }
}
