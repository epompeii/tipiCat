#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "cat.h"
#include "multiwaysprite.h"
#include "gamedata.h"
#include "manager.h"
#include "explodingSprite.h"
#include "multiwayexplodingsprite.h"


Manager::~Manager() { 
  for(unsigned i = 0; i < sprites.size(); ++i){
	 delete sprites[i] ;
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  mid("mid", Gamedata::getInstance().getXmlInt("mid/factor") ),
  fore("fore", Gamedata::getInstance().getXmlInt("fore/factor") ),
  hud(),
  viewport( Viewport::getInstance() ),
  sprites(),
  burm(),
  landed(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  hudUp(true),
  midground(Gamedata::getInstance().getXmlInt("snowflakes/mid/num")),
  background(Gamedata::getInstance().getXmlInt("snowflakes/back/num")),
  totalFlakes(midground+background),
  flakesPerSecond(Gamedata::getInstance().getXmlInt("snowflakes/perSecond")),
  flakesSince(1),

  snow(), 
  score(0),
  flakesCaught(0),

  sound()
 
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL) ;
  atexit(SDL_Quit) ;
  sprites.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  burm.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  landed.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  sprites.push_back(new Cat("tipiCat")) ;
  landed.push_back(0) ;
  for(int looper = 0 ; looper < totalFlakes ; ++looper){
  	sprites.push_back(new Sprite("snowflake", snow.snowfall(sprites[0]->X()), snow.snowRate("snowflake"))) ;
    landed.push_back(0) ;
 	  sprites.push_back(new MultiWaySprite("spinflake", snow.snowfall(sprites[0]->X()), snow.snowRate("spinflake"))) ;
    landed.push_back(0) ;
  }
  viewport.setObjectToTrack(sprites[currentSprite]) ;
}


bool Manager::checkForCat() {
  if(sprites[0]->Y() >= 419){
    return false ;
  }
  //int theX = sprites[0]->X() ;
  //int theY = sprites[0]->Y() ;

  for(int looper = background ; looper < totalFlakes ; ++looper){
    if(sprites[0]->collidedWith(sprites[looper])){
      if(landed[looper] == 0){
        //explode that snow flake
        landed[looper] = 2 ;
        if ( dynamic_cast<Sprite*>(sprites[looper]) ) {
          score += 25 ;
          Sprite *temp = static_cast<Sprite*>(sprites[looper]) ;
          sprites[looper] = new ExplodingSprite(*temp);
          delete temp;

          sound[0];
        }
        else{
          score += 50 ;
          MultiWaySprite *temp = static_cast<MultiWaySprite*>(sprites[looper]) ;
          sprites[looper] = new MultiWayExplodingSprite(*temp) ;
          delete temp;

          sound[0];
        }

        //score += 25 ;
        flakesCaught++ ;
        sprites[looper]->setPosition(Vector2f(sprites[looper]->X()+4240,0)) ; 
      }
      else if(landed[looper] == 1){
        // you lose
        return false ;
      }
    }
  }

  return true ;
}

bool Manager::checkForExp(){
  for(int looper = background ; looper < totalFlakes ; ++looper){
    if(landed[looper] == 2){
     if ( dynamic_cast<ExplodingSprite*>(sprites[looper]) ) {
        if ( static_cast<ExplodingSprite*>(sprites[looper])->chunkCount() == 0 ) {
          delete sprites[looper];
          sprites[looper] = new Sprite("snowflake", snow.snowfall(sprites[0]->X()), snow.snowRate("snowflake")) ;
          landed[looper] = 0 ;
        }
      }
      else if( dynamic_cast<MultiWayExplodingSprite*>(sprites[looper]) ){
        if ( static_cast<MultiWayExplodingSprite*>(sprites[looper])->chunkCount() == 0 ) {
          delete sprites[looper];
          sprites[looper] = new MultiWaySprite("spinflake", snow.snowfall(sprites[0]->X()), snow.snowRate("spinflake")) ;
         landed[looper] = 0 ;
        }
      }
    }
  }
  return true ;
}

bool Manager::checkForBurm() {
  //int theX = 0 ;
  int theY = 0 ;
  //for all foregorund sprites
  for(int looper = background ; looper < totalFlakes ; ++looper){
    //std::cout << "landed: " << landed[looper] << std::endl;
    //theX = sprites[looper]->X() ;
    theY = sprites[looper]->Y() ;
    //if not moving add to burm
    if(sprites[looper]->velocityX() == 0 && sprites[looper]->velocityY() == 0 && landed[looper] == 0){
      landed[looper] = 1 ;
      burm.push_back(sprites[looper]) ;
    }
    else if (landed[looper] == 0){
     // std::cout << "not landed" << std::endl;
      //check to see if moving hit the burm
      uint burmSize = burm.size() ;
       //std::cout << "Burm Size" << burmSize << std::endl;
      for(uint looperI = 0 ; looperI < burmSize ; ++looperI){
        //std::cout << "burm: " << burm[looperI]->Y() << std::endl;
        if(theY-burm[looperI]->Y() >= -64){
          //std::cout << "close: " << looper << std::endl;
           //check with midpoint collision
        //std::cout << "loc " << sprites[looper]->Y() << std::endl;
          if (sprites[looper]->collidedWith(burm[looperI])) {
            //std::cout << "colide: " << looper << std::endl;
            sprites[looper]->velocityX(0) ;
            sprites[looper]->velocityY(0) ;
            landed[looper] = 1 ;
            burm.push_back(sprites[looper]) ;
            //return true ;
          }
        }
      }
    }
  }
  return true;
}

void Manager::draw() const {
  back.draw() ;
  
  for(int looper = 1; looper < background; ++looper){
   sprites[looper]->draw() ;
  }

  mid.draw() ;
  
  for(int looper = background ; looper < totalFlakes ; ++looper){
	 sprites[looper]->draw() ;
  }

  sprites[0]->draw() ;

  fore.draw() ;

  io.printMessageCenteredAt(title, 10);
  io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("fullName"), 50);
  io.printMessageValueAt("Score: ", score, 525, 25);
  viewport.draw();

  if(landed[0] == 1){
    io.printMessageCenteredAt("You Lose!", 220);
    io.printMessageCenteredAt("Press R to restart.", 240);
  }
  else{
    if(hudUp == true){
      hud.drawHud(clock.getSeconds(),clock.getAvgFps(),totalFlakes,flakesCaught) ;
   }
  }
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "video/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  if(checkForCat()){
    ++clock;
    Uint32 ticks = clock.getElapsedTicks();


    checkForExp() ;
    checkForBurm() ;

    for(int looper = 0; looper < totalFlakes; ++looper){
  	 sprites[looper]->update(ticks, sprites[0]->X()) ;
    }

    if ( makeVideo && frameCount < frameMax ) {
      makeFrame();
    }

    back.update();
    mid.update();
    fore.update();

    if(flakesSince >= 1000/flakesPerSecond ){
      //std::cout << "Making flakes!" << std::endl;
      //std::cout << "Cat X():" << sprites[0]->X() << std::endl;
      if(flakesCaught == 0){
        sprites.push_back(new Sprite("snowflake", snow.snowfall(sprites[0]->X()), snow.snowRate("snowflake"))) ;
        landed.push_back(0) ;
        //std::cout << "Y IN MAN IS: " << sprites.back()->Y() << std::endl;
        sprites.push_back(new MultiWaySprite("spinflake", snow.snowfall(sprites[0]->X()), snow.snowRate("spinflake"))) ;
        landed.push_back(0) ;
       // std::cout << "Y IN MAN IS: " << sprites.back()->Y() << std::endl;
        totalFlakes += 2 ;
        flakesSince = 0 ;
      }
      else{
        flakesCaught-- ;
        flakesSince = 0 ;
      }
    }
    flakesSince += ticks ;
  }
  else{
    landed[0] = 1 ;
  }
  viewport.update();
}

void Manager::restart(){
 //std::vector<Drawable*>::iterator ptr = sprites.begin();
 /* while ( ptr != sprites.end() ) {
    ptr = sprites.erase(ptr);
  }*/

  for(unsigned i = 0; i < sprites.size(); ++i){
   delete sprites[i] ;
  }
  sprites.clear() ;

  burm.clear() ;
  landed.clear() ;

  snow.getFlakes(-1) ;
  score = 0 ;

  totalFlakes = midground + background ;

  sprites.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  burm.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  landed.reserve(Gamedata::getInstance().getXmlInt("snowflakes/max")) ;
  sprites.push_back(new Cat("tipiCat")) ;
  landed.push_back(0) ;
  for(int looper = 0 ; looper < totalFlakes ; ++looper){
    sprites.push_back(new Sprite("snowflake", snow.snowfall(sprites[0]->X()), snow.snowRate("snowflake"))) ;
    landed.push_back(0) ;
    sprites.push_back(new MultiWaySprite("spinflake", snow.snowfall(sprites[0]->X()), snow.snowRate("spinflake"))) ;
    landed.push_back(0) ;
  }
  viewport.setObjectToTrack(sprites[currentSprite]) ;
}

void Manager::play() {
	SDL_Event event;

	  bool done = false;
	  bool keyCatch = false;
	  while ( not done ) {

	    SDL_PollEvent(&event);
	    Uint8 *keystate = SDL_GetKeyState(NULL);
	    if (event.type ==  SDL_QUIT) { 
        done = true ;
        break; 
      }
	    if(event.type == SDL_KEYUP) { 
	      keyCatch = false; 
	    }
	    if(event.type == SDL_KEYDOWN) {
	      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
      		done = true;
      		break;
	      }
	      if (keystate[SDLK_SPACE] && !keyCatch) {
      		keyCatch = true;
      		sprites[0]->play(1) ;
	      }
    		if (keystate[SDLK_p] && !keyCatch) {
    		  keyCatch = true;
    		  if ( clock.isPaused() ) clock.unpause();
    		  else clock.pause();
    		}
        if (keystate[SDLK_m] && !keyCatch) {
          keyCatch = true;
          sound.toggleMusic();
        }
    		if (keystate[SDLK_F4] && !makeVideo) {
    		  std::cout << "Making video frames" << std::endl;
    		  makeVideo = true;
    		}
    		if (keystate[SDLK_F1] && !keyCatch) {
    			keyCatch = true;
    			if(hudUp == true){
    				hudUp = false ;
    			}
    			else{
    				hudUp = true ;
    			}
    		}
        if (keystate[SDLK_r] && !keyCatch) {
          keyCatch = true;
          restart() ;
        }
	    }

	    draw();
	    update();
	  }
}
