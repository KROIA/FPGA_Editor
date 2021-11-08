#include "gate.h"

sf::Font* Gate::m_font = nullptr;
vector<Gate *> Gate::m_globalGatelIst;

Gate::Gate(int inputs, int outputs)
{
    setup(inputs,outputs);
}
Gate::Gate(Vector2i pos,int inputs, int outputs)
{
    setup(inputs,outputs);
    m_position = pos;
}
void Gate::setup(int inputs, int outputs)
{
    m_globalGatelIst.push_back(this);
    //int inputPins = 4;
    //int outputPins = 2;
    m_position = Vector2i(100,150);



    if(inputs > outputs)
    {

        m_dimensions = Vector2i(m_gridsize.x*5,m_gridsize.y*(inputs*2));
    }else
    {
        m_dimensions = Vector2i(m_gridsize.x*5,m_gridsize.y*(outputs*2));
    }



    m_boundingBox.set(Vector2i(m_position.x-m_dimensions.x/2,m_position.y-m_dimensions.y/2),
                      m_position + m_dimensions);

    setupPins(inputs,outputs);
    m_moving = false;
    if(m_font == nullptr)
    {
        m_font = new sf::Font();
        if(!m_font->loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        {
            qDebug() << "ERROR: Gate::Gate() can't load Font";
        }
    }
    m_label.setFont(*m_font);
    m_label.setString("Gate");
    m_label.scale(0.1,0.1);
    m_label.setCharacterSize(150);
    //m_label.setScale(100,100);
    m_label.setFillColor(Color(20,20,20));
    m_label.setStyle(sf::Text::Bold);
}

Gate::~Gate()
{
    for(size_t i=0; i<m_globalGatelIst.size(); i++)
        if(m_globalGatelIst[i] == this)
            m_globalGatelIst.erase(m_globalGatelIst.begin() + i);

}

void Gate::onEventUpdate(sf::RenderWindow *window)
{

    if(m_moving)
    {
        //sf::Vector2i pixelPos = sf::Mouse::getPosition() - window->getPosition() - sf::Vector2i(window->getView().getViewport().getPosition());
        sf::Vector2i pixelPos = sf::Vector2i(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
        setPos(pixelPos);
    }

}
void Gate::onMouseOver(Vector2i mousePos)
{

}
void Gate::onKlick(sf::Mouse::Button mouseButton, Vector2i mousePos)
{
    if(Tool::getSelectedType() == Tool::Type::moveModifier &&
       mouseButton == sf::Mouse::Button::Left)
    {
        if(!Tool::getSelected().isUsed())
        {
            Tool::getSelected().isUsed(true);
            Tool::startListen(this);
            m_moving = true;
            m_movingOldPos = m_position;
        }
        else if(m_moving)
        {
            Tool::getSelected().isUsed(false);
            Tool::endListen(this);
            m_moving = false;
        }
    }
}


void Gate::setupPins(int inputs, int outputs)
{

    int pinSpacing;
    if(inputs > outputs)
        pinSpacing = m_dimensions.y / (inputs);
    else
        pinSpacing = m_dimensions.y / (outputs);

   // int inputPinPosSpacing  = m_dimensions.y / (inputs);
   // int outputPinPosSpacing = m_dimensions.y / (outputs);

    for(int i=0; i<inputs; i++)
    {
        Pin *pin = new Pin(Pin::Type::input,180);

        Vector2i pos(-m_dimensions.x/2,
                     - ((inputs-1)*pinSpacing)/2 + pinSpacing*i);
        pin->setParent(this);
        pin->setPos(pos);
        m_inputPins.push_back(pin);
    }
    for(int i=0; i<outputs; i++)
    {
        Pin *pin = new Pin(Pin::Type::output,0);

        Vector2i pos(m_dimensions.x/2,
                     - ((outputs-1)*pinSpacing)/2 + pinSpacing*i);
        pin->setParent(this);
        pin->setPos(pos);
        pin->addToBlacklist(m_inputPins);
        m_outputPins.push_back(pin);
    }

    for(int i=0; i<inputs; i++)
    {
        m_inputPins[i]->addToBlacklist(m_outputPins);
    }

}
void Gate::processLogic()
{

}

void Gate::draw(sf::RenderWindow *window, Vector2i drawPos)
{
    if(m_parent != nullptr)
    {
        m_isVisible = m_parent->isVisible();
        drawPos += m_parent->getPos();
    }
    if(!m_isVisible)
        return;
    drawPos += m_position;
    m_boundingBox.set(Vector2i(drawPos.x-m_dimensions.x/2,drawPos.y-m_dimensions.y/2),
                      Vector2i(drawPos.x+m_dimensions.x/2,drawPos.y+m_dimensions.y/2));



    /*if(m_parent != nullptr)
    {
        drawPos + m_parent->getPos();
    }*/

    sf::ConvexShape convex;

    // resize it to 5 points
    convex.setPointCount(4);

    // define the points
    convex.setPoint(0, sf::Vector2f(-m_dimensions.x/2, -m_dimensions.y/2));
    convex.setPoint(1, sf::Vector2f( m_dimensions.x/2, -m_dimensions.y/2));
    convex.setPoint(2, sf::Vector2f( m_dimensions.x/2,  m_dimensions.y/2));
    convex.setPoint(3, sf::Vector2f(-m_dimensions.x/2,  m_dimensions.y/2));

    convex.setFillColor(sf::Color(80,80,80));
    convex.setPosition((Vector2f(drawPos)));

    sf::FloatRect labelBounds = m_label.getGlobalBounds();
    m_label.setOrigin(labelBounds.width/2/m_label.getScale().x,
                      (labelBounds.height-labelBounds.height/10)/m_label.getScale().y);
    m_label.setPosition((Vector2f(drawPos)));
   // qDebug() << "Bounds: "<<labelBounds.width << " " <<labelBounds.height;

    /**Color col(255,0,0);
    sf::Vertex line[] =
    {
        sf::Vertex(Vector2f(drawPos),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(labelBounds.width,0)),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(labelBounds.width,labelBounds.height)),col),
        sf::Vertex(Vector2f(drawPos+Vector2i(0,labelBounds.height)),col)
    };
*/

   /* for(size_t i=0; i<m_inputPins.size(); i++)
    {
       // qDebug() << "drawPin";
        m_inputPins[i].draw(window,drawPos);
    }
    for(size_t i=0; i<m_outputPins.size(); i++)
    {
        m_outputPins[i].draw(window,drawPos);
    }*/

    window->draw(convex);
    window->draw(m_label);
   // window->draw(line,4,sf::LineStrip);
}

void Gate::global_processLogic()
{
    for(size_t i=0; i<m_globalGatelIst.size(); i++)
        m_globalGatelIst[i]->processLogic();
}
void Gate::toolChanged(Tool *oldTool, Tool *newTool)
{
    toolCleared(oldTool);
}
void Gate::toolCleared(Tool *oldTool)
{
    Tool::endListen(this);
    m_moving = false;
    setPos(m_movingOldPos);
}

