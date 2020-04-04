#include "scad.h"

using namespace scad;

int main() {
  Union(Cube(10), Sphere(5).Translate(0, 0, 5)).WriteToFile("example.scad");
}
