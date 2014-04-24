#include <cmath>
#include "cat.h"
#include "gamedata.h"
#include "frameFactory.h"

void Cat::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Cat::Cat( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt("tipiCat/loc/x"), 
                    Gamedata::getInstance().getXmlInt("tipiCat/loc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt("tipiCat/speedX"),
                    Gamedata::getInstance().getXmlInt("tipiCat/speedY"))
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  hasMoved(false),

  //set up in xml
  angleDegrees(45),
  angle(static_cast<double>(angleDegrees) * 3.14159 / 180.0),
  g(9.81),
  velocity(10.0),
  DT(.001),
  Vx(velocity * cos(angle)),
  Vy(velocity * sin(angle)),
  t(-1),
  t2(-1),

  strategyR(new RectangularCollisionStrategy ),
  strategyP(new PerPixelCollisionStrategy)
{ }

Cat::Cat(const Cat& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  hasMoved(s.hasMoved),

  angleDegrees(s.angleDegrees),
  angle(s.angle),
  g(s.g),
  velocity(s.velocity),
  DT(s.DT),
  Vx(s.Vx),
  Vy(s.Vy),
  t(s.t),
  t2(s.t2),

  strategyR( new RectangularCollisionStrategy(*s.strategyR) ),
  strategyP( new PerPixelCollisionStrategy(*s.strategyP))
  { }

void Cat::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

Cat& Cat::operator=(const Cat& rhs) {
  Drawable::operator=( rhs );
  //frames = rhs.frames;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  strategyR = rhs.strategyR ;
  strategyP = rhs.strategyP ;
  return *this;
}

void Cat::update(Uint32 ticks) { 
   //std::cout << ticks << std::endl;
  advanceFrame(ticks);
  
  if(t >= 0){
    jump(ticks) ;
  }
  else if( t2 >= 0){
    fall(ticks) ;
  }
  else{
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
      setPosition(getPosition() + incr);

      if ( Y() < 0) {
        velocityY( abs( velocityY() ) );
      }
      if ( Y() > worldHeight-frameHeight) {
        velocityY( -abs( velocityY() ) );
      }

      if ( X() < 0) {
        velocityX( abs( velocityX() ) );
      }
      if ( X() > worldWidth-frameWidth) {
        velocityX( -abs( velocityX() ) );
      }  
  }
}

void Cat::update(Uint32 ticks, float catAt) { 
   //std::cout << ticks << std::endl;
  advanceFrame(ticks);
  
  if(t >= 0){
    jump(ticks) ;
  }
  else if( t2 >= 0){
    fall(ticks) ;
  }
  else{
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
      setPosition(getPosition() + incr);

      if ( Y() < 0) {
        velocityY( abs( velocityY() ) );
      }
      if ( Y() > worldHeight-frameHeight) {
        velocityY( -abs( velocityY() ) );
      }

      if(catAt == Y()){
        // std::cout << true << std::endl;
      }  
  }
}

void Cat::play(const int moveIn){
	//bool hasMoved = false ;
	if(moveIn == 1){
		hasMoved = true ;
    t = 0 ; 
    t2 = -1 ;
	}
}

void Cat::jump(Uint32 ticks){
  t += ticks * DT ;
  double x = Vx * t;
  double y = Vy * t - 0.5 * g * t * t;

  //std::cout << "x:" << x << std::endl;
  //std::cout << "y:" << y << std::endl;
  //std::cout << "X():" << X() << std::endl;
  //std::cout << "Y():" << Y() << std::endl;
  
  if( Y() > 420){
    setPosition(Vector2f(X(),419));
    setVelocity(Vector2f(Gamedata::getInstance().getXmlInt("tipiCat/speedX"),0)) ;
  }
  else if(Y() > 0){
    Vector2f vel = Vector2f(x,-1*y) ;
    setPosition(getPosition() + vel);
  }
  else{
    t = -1 ;
    t2 = 0 ;
  }
  
}

void Cat::fall(Uint32 ticks){
  t2 += ticks * DT  ;
  double x = Vx * t2;
  double y = Vy * t2 - 0.5 * g * t2 * t2;

 // std::cout << "x:" << x << std::endl;
  //std::cout << "y:" << y << std::endl;
 // std::cout << "X():" << X() << std::endl;
  //std::cout << "Y():" << Y() << std::endl;
  if( Y() > 420){
    setPosition(Vector2f(X(),419));
    setVelocity(Vector2f(Gamedata::getInstance().getXmlInt("tipiCat/speedX"),0)) ;
      t2 = -1 ;
  }
  else if( Y() > 42){
     setVelocity(Vector2f(25,200));
     Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
      setPosition(getPosition() + incr);
  }
  else if(Y() < 450){
    Vector2f vel = Vector2f(x,y) ;
    setPosition(getPosition() + vel);
  }
  else{
    setVelocity(Vector2f(Gamedata::getInstance().getXmlInt("tipiCat/speedX"),0)) ;
     t = -1 ;
     t2 = -1 ;
  }
  
}

bool Cat::collidedWith(const Drawable* d) { 
   if(strategyR->execute(*this, *d)){
    return strategyP->execute(*this, *d) ;
   }
   else{
    return false ;
   }
}

