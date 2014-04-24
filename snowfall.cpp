#include "snowfall.h"
#include "gamedata.h"


Snowfall::Snowfall() {}


Vector2f Snowfall::snowfall(float loc){
	 //std::cout << "loc of flake" << loc << std::endl;
	//static int snowStart = loc + (rand()%Gamedata::getInstance().getXmlInt("view/width")) + Gamedata::getInstance().getXmlInt("view/width");
	static int snowStart = 0 ;
	static bool snowMax = false ;
	static bool snowBlow = false ;

	if(getFlakes(0) >= Gamedata::getInstance().getXmlInt("snowflakes/back/num") && snowMax == false){
		snowStart = 0 ;
		snowMax = true ;
	}
	else if(getFlakes(0) - Gamedata::getInstance().getXmlInt("snowflakes/back/num") >= Gamedata::getInstance().getXmlInt("snowflakes/mid/num") && snowMax == true ){
		snowBlow = true ;
	}

	if(snowMax == false){
		snowStart += (int)Gamedata::getInstance().getXmlInt("back/width")/Gamedata::getInstance().getXmlInt("snowflakes/back/num")*2 ;
	}
	else if(snowBlow == false){
		snowStart += (int)Gamedata::getInstance().getXmlInt("back/width")/Gamedata::getInstance().getXmlInt("snowflakes/mid/num")*2 ;
		snowStart += loc ;
		//std::cout << "Making flakes! " << loc << std::endl;
	}
	else{
		snowStart = loc + rand()%4240 ;
		//std::cout << "New flakes! " << snowStart << std::endl;
		 //std::cout << "New flake" << snowStart << std::endl;
	}

	Vector2f snow(snowStart,0) ;

	return snow ;
}

Vector2f Snowfall::snowRate(const std::string& name){
	int snowSpeedX = (rand()%Gamedata::getInstance().getXmlInt(name+"/speed/x"))+ 1 ;
	int snowSpeedY = (rand()%Gamedata::getInstance().getXmlInt(name+"/speed/y"))+ 1 ;
	Vector2f snow(snowSpeedX,snowSpeedY) ;
	return  snow ;	
}

int Snowfall::getAngle() const{
	return rand()%90 ;
}

float Snowfall::getScale() const{
	if(getFlakes(3) < Gamedata::getInstance().getXmlInt("snowflakes/back/num")){
			return (rand()%5)*.1 ;
	}
	else{
		return ((rand()%5)*.1)+.7 ;
	}
	
}

int Snowfall::getFlakes(int who) const{
	static int flakes = 0 ;
	if(who == 3){
		flakes++ ;
	}
	else if(who == -1){
		flakes = 0 ;
	}
	return flakes-1 ;
}
