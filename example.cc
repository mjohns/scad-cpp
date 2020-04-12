#include "scad.h"

#include <vector>

using namespace scad;

int main() {
  Shape s = Union(Cube(10), Sphere(5).Translate(0, 0, 5));

  std::vector<Shape> shapes;
  for (int i = 0; i < 3; ++i) {
    shapes.push_back(s.TranslateX(15 * i));
  }

  shapes.push_back(Shape::LiteralPrimitive("cube(2);").TranslateX(45));

  Shape cylinder = Cylinder(1, 2, 20).Projection().LinearExtrude(30);

  (UnionAll(shapes) - cylinder).WriteToFile("example.scad");
}
