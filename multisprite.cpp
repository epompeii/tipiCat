#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, snow.snowfall(Y()), snow.snowRate(name)),
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y,0,scale,false);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

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

void MultiSprite::update(Uint32 ticks, float catAt) {
  advanceFrame(ticks);
  
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
        setPosition(Vector2f(X()+4240,Y())); 
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
}
