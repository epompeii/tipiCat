#include <iostream>
#include "multiwaysprite.h"
#include "gamedata.h"

class MultiChunk : public MultiWaySprite {
public:
  explicit MultiChunk(
    const Vector2f& pos, const Vector2f vel, 
    const string& name, const Frame* fm) :
    MultiWaySprite(name, pos, vel, fm), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/chunk/distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
private:
  float distance;
  float maxDistance;
  bool tooFar;
};
