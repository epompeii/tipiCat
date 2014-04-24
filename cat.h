#ifndef Cat__H
#define Cat__H
#include <string>
#include <vector>
#include "drawable.h"
#include "collisionStrategy.h"

class Cat : public Drawable {
public:
  Cat(const std::string&);
  Cat(const Cat&);
  virtual ~Cat(){ if(strategyR){ delete strategyR ;} 
      if (strategyP){ delete strategyP ; } }  
  Cat& operator=(const Cat&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void update(Uint32 ticks, float catAt);
  
  virtual const Frame* getFrame() const {
    return frames[currentFrame]; 
  }

 virtual void play(const int moveIn) ;

 virtual bool collidedWith(const Drawable*) ;

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  void jump(Uint32 ticks) ;
  void fall(Uint32 ticks) ;
  bool hasMoved ;

  int angleDegrees ;
  double angle ;
  double g ;
  double velocity ;
  double DT ;
  double Vx ;
  double Vy ;
  double t ;
  double t2 ;

  RectangularCollisionStrategy * strategyR;
  PerPixelCollisionStrategy * strategyP;
};
#endif
