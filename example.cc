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

  Shape pyramid = Polyhedron({{10, 10, 0}, {10, -10, 0}, {-10, -10, 0}, {-10, 10, 0}, {0, 0, 10}},
                             {{0, 1, 4}, {1, 2, 4}, {2, 3, 4}, {3, 0, 4}, {1, 0, 3}, {2, 1, 3}});

  shapes.push_back(pyramid.TranslateY(-25));

  shapes.push_back(RegularPolygon(6, 3).LinearExtrude(10).Translate(20, -25, 0));

  (UnionAll(shapes) - cylinder).WriteToFile("example.scad");
}
