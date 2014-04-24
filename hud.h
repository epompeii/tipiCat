#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"


class Hud {
public:
  Hud();
  void drawHud(int seconds, int fps, int flakes, int pool) const;
private:
  const IOManager& io;
  SDL_Surface * const screen;

  Vector2f start;
  int totalLength;
  int totalWidth;
  
 // const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
 
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
