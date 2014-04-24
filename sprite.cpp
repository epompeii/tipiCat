#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"


Sprite::Sprite(const std::string& name) : 
  Drawable(name, snow.snowfall(Y()), snow.snowRate(name)),
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
   strategy( new MidPointCollisionStrategy )
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
   strategy( new MidPointCollisionStrategy )
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
   strategy( new MidPointCollisionStrategy )
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
   strategy( new MidPointCollisionStrategy(*s.strategy) )
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  strategy = new MidPointCollisionStrategy(*rhs.strategy) ;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y,angle,scale,true); 
}

void Sprite::drawChunk() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
    if(scale > .5 ){
      velocityX( 0 );
      velocityY( 0 );
    }
    else{
      setPosition(Vector2f(X(),0)); 
    }
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}

void Sprite::update(Uint32 ticks, float catAt) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
    if(scale > .5 ){
      if(X() < catAt - 320){
        //std::cout << "CatAt:" << catAt << std::endl;
       // std::cout << "X():" << X() << std::endl;
        //std::cout << "Move To:" << X()+4240 << std::endl;
        velocityX( 0 );
        velocityY( 0 );
       // std::cout << "old flake down" << Y() << std::endl;
        setPosition(Vector2f(X()+4240,Y())); 
       // std::cout << "move flake downed" << Y() << std::endl;
      }
      else{
        velocityX( 0 );
        velocityY( 0 );
      }
    }
    else{
      if(X() < catAt - 320){
       // std::cout << "CatAt:" << catAt << std::endl;
       // std::cout << "X():" << X() << std::endl;
       // std::cout << "Move To:" << X()+4240 << std::endl;
        setPosition(Vector2f(X()+4240,0)); 
      }
      else{
        setPosition(Vector2f(X(),0)); 
      }
    }
  }
  if(scale > .5 ){
      if(X() < catAt - 320){
        //std::cout << "old flake" << X() << std::endl;
       //std::cout << "old flake" << Y() << std::endl;
        setPosition(Vector2f(X()+4240,Y())); 
        //std::cout << "move flake" << X() << std::endl;
       //  std::cout << "move flake" << Y() << std::endl;
      }
  }
}

bool Sprite::collidedWith(const Drawable* d) { 
   //std::cout << "here to sprite?" << std::endl;
    return strategy->execute(*this, *d);
}