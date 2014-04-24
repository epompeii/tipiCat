#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"
#include "snowfall.h"
#include "collisionStrategy.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Sprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const Sprite& s);
  virtual ~Sprite() { if(strategy) delete strategy ; } 
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void drawChunk() const;

  virtual void update(Uint32 ticks);
  virtual void update(Uint32 ticks, float catAt);

  virtual bool collidedWith(const Drawable*) ;

private:
  Snowfall snow ;
  int angle ;
  float scale ;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  MidPointCollisionStrategy * strategy;

  int getDistance(const Sprite*) const;
};
#endif
