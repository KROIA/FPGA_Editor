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

}
Canvas::~Canvas()
{

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

    Shape::global_draw(this);

#ifdef DEBUG_MODE
    Shape::global_drawDebug(this);
#endif
}

void Canvas::setPosition(QPoint &pos)
{
    move(pos);
}
void Canvas::setSize(QSize &size)
{
   /* sf::View view = this->getView();
    Vector2i pos(size.width(),size.height());
    QSize oldSize = geometry().size();
    sf::FloatRect oldViewArea = view.getViewport();*/

    float aspectRatio = (float)this->getSize().x / (float)this->getSize().y;
    //this->getView().getSize()


    /*sf::FloatRect visibleArea(view.getViewport().getPosition().x, view.getViewport().getPosition().y,
                                  size.width() * oldViewArea.width / oldSize.width(),
                                  size.height()* oldViewArea.height / oldSize.height());
*/
//  sf::FloatRect visibleArea(view.getViewport().getPosition().x, view.getViewport().getPosition().y,
 //                             this->mapPixelToCoords(pos).x,this->mapPixelToCoords(pos).y);

  //  view.setViewport(visibleArea);

    resize(size);
    sf::View view = this->getView();
    view.setSize((float)this->getSize().y * aspectRatio * m_zoomValue, this->getSize().y * m_zoomValue);

    //this->setView(sf::View(visibleArea));
    this->setView(view);
}
Vector2i Canvas::getMousePosition()
{
    if(m_instance != nullptr)
        return sf::Mouse::getPosition() - m_instance->getPosition();
    return Vector2i(0,0);
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
                sf::Vector2f deltaPos = m_movingOldPos - newPos;

                // Applying zoom "reduction" (or "augmentation")
                deltaPos.x *= m_zoomValue;
                deltaPos.y *= m_zoomValue;

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

    Shape::global_updateEvents(&events,this);
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
