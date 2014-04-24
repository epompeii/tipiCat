#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "aaline.h"
#include "hud.h"
#include "snowfall.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;

  World back ;
  World mid ;
  World fore ;
  Hud hud ;

  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> burm;
  std::vector<int> landed ;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  std::string username;
  std::string title;
  int frameMax;

  bool hudUp ;
  int midground ;
  int background ;
  int totalFlakes ;
  int flakesPerSecond ;
  int flakesSince ;

  Snowfall snow ;
  int score ;
  int flakesCaught ;

  SDLSound sound;

  bool checkForCat() ;
  bool checkForExp() ;
  bool checkForBurm() ;
  //bool checkForFlake() const ;
  //bool checkForFail() const ;
  void draw() const;
  void update();
  void restart() ;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  void drawBox() const ;

};
