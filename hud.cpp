#include "hud.h"
#include "gamedata.h"

Hud::Hud() :
  io( IOManager::getInstance() ),
  screen( io.getScreen() ),

  start(Vector2f(Gamedata::getInstance().getXmlInt("hud/srcX"), 
	Gamedata::getInstance().getXmlInt("hud/srcY"))), 
  totalLength(Gamedata::getInstance().getXmlInt("hud/length")), 
  totalWidth(Gamedata::getInstance().getXmlInt("hud/width")), 
  
 // RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) )
 {}


void Hud::drawHud(int seconds, int fps, int flakes, int pool) const {

  Draw_AALine(screen, start[0], start[1]+totalWidth/2, 
                      start[0]+totalLength, start[1]+totalWidth/2, 
                      116.0, GRAY);
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8, 
                      start[0]+totalLength, start[1]-8, 
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+totalWidth+8, 
                      start[0]+totalLength, start[1]+totalWidth+8, 
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8, 
                      start[0]-1, start[1]+totalWidth+8, 
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8, 
                      start[0]+totalLength+1, start[1]+totalWidth+8, 
                      2.0, BLACK);

  io.printMessageValueAt("Seconds: ", seconds, 15, 25);
  io.printMessageValueAt("fps: ", fps, 15, 45);
  io.printMessageValueAt("Flakes: ", flakes, 15, 65);
  io.printMessageValueAt("Pool: ", pool, 125, 65);
  io.printMessageAt("Tap Spacebar to jump.", 15, 85);
  io.printMessageAt("Catch the snowfall!", 15, 105);

}

