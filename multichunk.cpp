#include <iostream>
#include <cmath>

#include "multichunk.h"

void MultiChunk::update(Uint32 ticks) { 
  float yincr = velocityY() * static_cast<float>(ticks) * 0.001;
  Y( Y()- yincr );
  float xincr = velocityX() * static_cast<float>(ticks) * 0.001;
  X( X()- xincr );
  distance += ( hypot(xincr, yincr) );
  if (distance > maxDistance) tooFar = true;
}

