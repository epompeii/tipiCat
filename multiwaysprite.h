#ifndef MULTIWAYSPRITE__H
#define MULTIWAYSPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "snowfall.h"
#include "collisionStrategy.h"


class MultiWaySprite : public Drawable {
public:
  MultiWaySprite(const std::string&);
  MultiWaySprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  MultiWaySprite(const MultiWaySprite&);
  MultiWaySprite(const std::string& name, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  virtual ~MultiWaySprite() { if(strategy) delete strategy ; }

  MultiWaySprite& operator=(const MultiWaySprite&);

  virtual void draw() const;
  virtual void drawChunk() const;
  virtual void update(Uint32 ticks);
  virtual void update(Uint32 ticks, float catAt);
  
  virtual const Frame* getFrame() const {
    return frames[currentFrame]; 
  }

  virtual bool collidedWith(const Drawable*) ;

protected:
  Snowfall snow ;
  int angle ;
  float scale ;
  const std::vector<Frame *> frames;
  const std::vector<Frame *> framesBack;
  bool direction ;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  MidPointCollisionStrategy * strategy;

  void advanceFrame(Uint32 ticks);
};
#endif
