#include"QuadTree.h"

int main()
{
  // Create quadtree
  QuadTree* qtree = new QuadTree(Quad(0.0f, 0.0f, 100.0f, 100.0f));
  
  // Insert point in quadtree
  qtree->Insert(Point(10.0f, 12.0f));
  
  // Get all the points inside quad with size 20, 20 and coords 0, 0
  std::vector<Point> pointsInRangeQuad = qtree->Query(Quad(0.0f, 0.0f, 20.0f, 20.0f));
  
  return 0;
}
