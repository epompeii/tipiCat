#include <vector>
#include <list>
#include "multichunk.h"

class MultiWayExplodingSprite : public MultiWaySprite {
public:
  MultiWayExplodingSprite(const MultiWaySprite& s);
  ~MultiWayExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void update(Uint32 ticks, float catAt);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<MultiChunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<MultiChunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  MultiWayExplodingSprite(const MultiWayExplodingSprite&); // Explicit disallow (Item 6)
  MultiWayExplodingSprite& operator=(const MultiWayExplodingSprite&); // (Item 6)
};
