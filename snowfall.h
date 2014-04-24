#ifndef SNOWFALL__H
#define SNOWFALL__H
#include <string>
#include "vector2f.h"

class Snowfall{
public:
	Snowfall() ;
	Vector2f snowfall(float loc) ;
	Vector2f snowRate(const std::string& name) ;
	int getAngle() const;
	float getScale() const;
	int getFlakes(int who) const ;	
};

#endif
