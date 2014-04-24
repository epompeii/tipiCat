#include "multiwaysprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void MultiWaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiWaySprite::MultiWaySprite(const std::string& name) :
  Drawable(name, snow.snowfall(Y()), snow.snowRate(name)),
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames( FrameFactory::getInstance().getFrames(name) ),
  framesBack( FrameFactory::getInstance().getFrames(name+"Back") ),
  direction(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  strategy( new MidPointCollisionStrategy )
{ }

MultiWaySprite::MultiWaySprite(const string& name, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name, pos, vel), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames( FrameFactory::getInstance().getFrames(name) ),
  framesBack( FrameFactory::getInstance().getFrames(name+"Back") ),
  direction(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  strategy( new MidPointCollisionStrategy )
{}

MultiWaySprite::MultiWaySprite(const string& name, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(name, pos, vel), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames( FrameFactory::getInstance().getChunkFrames(frm) ),
  framesBack( FrameFactory::getInstance().getChunkFrames(frm) ),
  direction(true),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  strategy( new MidPointCollisionStrategy )
{}

MultiWaySprite::MultiWaySprite(const MultiWaySprite& s) :
  Drawable(s), 
  snow(),
  angle(snow.getAngle()),
  scale(snow.getScale()),
  frames(s.frames), framesBack(s.framesBack), direction(s.direction),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  strategy( new MidPointCollisionStrategy(*s.strategy) )
  { }

MultiWaySprite& MultiWaySprite::operator=(const MultiWaySprite& rhs) {
  Drawable::operator=( rhs );
  //frames = rhs.frames;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  strategy = new MidPointCollisionStrategy(*rhs.strategy) ;
  return *this;
}

void MultiWaySprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if(direction){
  	frames[currentFrame]->draw(x, y,0,scale,false);
  }
  else{
	framesBack[currentFrame]->draw(x, y,0,scale,false);
  }
}

void MultiWaySprite::drawChunk() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiWaySprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
	direction = false ;
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
	  direction = true ;
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
    	direction = false ;
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
	direction = true ;
    velocityX( -abs( velocityX() ) );
  }  

}

void MultiWaySprite::update(Uint32 ticks, float catAt) {
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
  if(scale > .5 ){
      if(X() < catAt - 320){
        setPosition(Vector2f(X()+4240,Y())); 
      }
  }
}

bool MultiWaySprite::collidedWith(const Drawable* d) { 
   //std::cout << "here to sprite?" << std::endl;
    return strategy->execute(*this, *d) ;
}