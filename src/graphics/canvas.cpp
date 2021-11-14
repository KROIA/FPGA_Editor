#include "canvas.h"
#include <iostream>
#include <string>
#include <QDir>

Canvas *Canvas::m_instance = nullptr;

Canvas::Canvas(QWidget* Parent,
                   const QPoint& Position,
                   const QSize& Size
                   /*vector<Shape*> *shapes*/)
    : QSFMLCanvas(Parent, Position, Size)
{
   // m_shapes = shapes;
    m_zoomValue = 1;
    m_instance = this;
    m_moving = false;

    m_shapeList.push_back(vector<Shape*> ());
    m_shapeList.push_back(vector<Shape*> ());
    m_shapeList.push_back(vector<Shape*> ());
    m_shapeList.push_back(vector<Shape*> ());
    m_shapeList.push_back(vector<Shape*> ());

}
Canvas::~Canvas()
{

}
void Canvas::addShape(size_t layer,Shape *shape)
{
    if(m_shapeList.size()>layer)
    {
        m_shapeList[layer].push_back(shape);
    }
    else
    {
        qDebug() << "Layer to high: "<<layer;
    }
}
void Canvas::removeShape(Shape *shape)
{
    for(size_t j=0; j<m_shapeList.size(); j++)
    {
        for(size_t i=0; i<m_shapeList[j].size(); i++)
        {
            if(m_shapeList[j][i] == shape)
            {
                m_shapeList.erase(m_shapeList.begin()+i);
                if(i>0)
                    i--;
            }
        }
    }
}
void Canvas::clearShape(size_t layer)
{
    if(m_shapeList.size()>layer)
    {
        m_shapeList[layer].clear();
    }
}
void Canvas::clearShape()
{
    for(size_t i=0; i<m_shapeList.size(); i++)
    {
        m_shapeList[i].clear();
    }
}
void Canvas::OnInit()
{
    // Load the image
    //std::cout << "onInit" << std::endl;
    //QString dir = QDir::currentPath();
    /*std::string utf8_text = dir.toUtf8().constData();
    std::cout << "HELLO: " << utf8_text << std::endl;
    if (!myImage.loadFromFile(utf8_text + "/chef.png")) {
        std::cout << "Loading error"<< std::endl;
    } else {
        std::cout << "Image was loaded fine" << std::endl;
    }
    // Setup the sprite
    mySprite.setTexture(myImage);
    mySprite.setPosition(150, 150);*/
   // std::cout << "setting the texture of the sprite" << std::endl;
    //mySprite.setCenter(mySprite.GetSize() / 2.f);
    //myClock.restart();
}
void Canvas::OnUpdate()
{
    // Clear screen
    RenderWindow::clear(sf::Color(255, 255, 255));

    updateEvents();

    for(size_t j=0; j<m_shapeList.size(); j++)
    {
        for(size_t i=0; i<m_shapeList[j].size(); i++)
        {
            m_shapeList[j][i]->draw(this);
        }
    }

    //Shape::global_draw(this);

#ifdef DEBUG_MODE
    for(size_t j=0; j<m_shapeList.size(); j++)
    {
        for(size_t i=0; i<m_shapeList[j].size(); i++)
        {
            m_shapeList[j][i]->drawDebug(this);
        }
    }
    //Shape::global_drawDebug(this);
#endif
}

void Canvas::setPosition(QPoint &pos)
{
    move(pos);
}
void Canvas::setSize(QSize &size)
{ 
    float aspectRatio = (float)size.width() / (float)size.height();

    resize(size);
    sf::View view = this->getView();
    view.setSize((float)this->getSize().y * aspectRatio * m_zoomValue, this->getSize().y * m_zoomValue);

    this->setView(view);
}
Vector2i Canvas::getMousePosition()
{
    if(m_instance != nullptr)
        return sf::Mouse::getPosition() - m_instance->getPosition();
    return Vector2i(0,0);
}

void Canvas::mouseDoubleClickEvent(QMouseEvent * e)
{
    m_doubleClickEvent = true;
}
void Canvas::updateEvents()
{
    vector<sf::Event> events;
    sf::Event e;
    const float zoomAmount{ 1.1f }; // zoom by 10%

    checkKeyEvents();
    emit checkKeyEvents();
    while(this->pollEvent(e))
    {
        events.push_back(e);

        switch(e.type)
        {
            case sf::Event::Closed:
            {
                this->sf::RenderWindow::close();
                break;
            }
            case sf::Event::KeyReleased:
            {
                //onKeyReleaseEvent(&e);
                break;
            }
            case sf::Event::KeyPressed:
            {
                //onKeyPressEvent(&e);
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                if (e.mouseWheelScroll.delta > 0)
                    zoomViewAt({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, this, (1.f / zoomAmount));
                else if (e.mouseWheelScroll.delta < 0)
                    zoomViewAt({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, this, zoomAmount);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                // Mouse button is pressed, get the position and set moving as active
                if (e.mouseButton.button == sf::Mouse::Button::Middle) {
                    m_moving = true;
                    m_movingOldPos = Vector2f(sf::Mouse::getPosition(*this)); // No call to mapPixelToCoords
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (e.mouseButton.button == sf::Mouse::Button::Middle)
                    m_moving = false;
                break;
            }
            case sf::Event::MouseMoved:
            {
                // Ignore mouse movement unless a button is pressed (see above)
                if (!m_moving)
                    break;
                // Determine the new position in world coordinates
                sf::Vector2f newPos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
                // Determine how the cursor has moved
                // Swap these to invert the movement direction

                //this->mapPixelToCoords(sf::Vector2i(m_movingOldPos*100.f - newPos*100.f))/100.f;
                sf::Vector2f deltaPos = (this->mapPixelToCoords(sf::Vector2i(m_movingOldPos*100.f))-
                                         this->mapPixelToCoords(sf::Vector2i(newPos*100.f)))/100.f;

                // Applying zoom "reduction" (or "augmentation")
               // deltaPos.x *= m_zoomValue;
               // deltaPos.y *= m_zoomValue;

                // Move our view accordingly and update the window
                sf::View view = this->getView();
                view.move(deltaPos); // <-- Here I use move
                this->setView(view);

                // Save the new position as the old one
                // We're recalculating this, since we've changed the view
                m_movingOldPos = newPos; // With move, I don't need to recalculate
                break;
            }
        }


    }

    for(size_t j=0; j<m_shapeList.size(); j++)
    {
        for(size_t i=0; i<m_shapeList[j].size(); i++)
        {
            m_shapeList[j][i]->updateEvents(&events,m_doubleClickEvent,this);
        }
    }
    m_doubleClickEvent = false;
    //Shape::global_updateEvents(&events,this);
}
void Canvas::p_checkKeyEvents()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
    {
        this->setView(this->getDefaultView());
    }
}

/*void Canvas::onKeyPressEvent(sf::Event *e)
{
    qDebug()<< "Key event: "<<e->key.code;
    switch(e->key.code)
    {
        case sf::Keyboard::BackSpace:
        {
            this->setView(this->getDefaultView());
            qDebug() << "reset view";
            break;
        }
    }
}
void Canvas::onKeyReleaseEvent(sf::Event *e)
{
    qDebug()<< "Key event: "<<e->key.code;
}*/

void Canvas::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow* window, float zoom)
{
    const sf::Vector2f beforeCoord{ window->mapPixelToCoords(pixel) };
    sf::View view{ window->getView() };
    view.zoom(zoom);
    m_zoomValue *= zoom;
    window->setView(view);
    const sf::Vector2f afterCoord{ window->mapPixelToCoords(pixel) };
    const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
    view.move(offsetCoords);
    window->setView(view);
}
