#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#define SCAD_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define SCAD_WARN_UNUSED_RESULT
#endif

namespace scad {

using ScadWriter = std::function<void(std::FILE* file, int indent_level)>;

struct LinearExtrudeParams {
  double height = 0;
  double twist = 0;
  double convexity = 10;
  int slices = 20;
  double scale = 1;
  bool center = true;
};

class Shape {
 public:
  Shape() {}
  explicit Shape(std::shared_ptr<ScadWriter> scad) : scad_(std::move(scad)) {}

  void WriteToFile(const std::string& file_name) const;
  void AppendScad(std::FILE* file, int indent_level) const;

  Shape Translate(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;

  Shape Mirror(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;

  Shape Rotate(double degrees,
               double x,
               double y,
               double z) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateX(double degrees) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateY(double degrees) const SCAD_WARN_UNUSED_RESULT;
  Shape RotateZ(double degrees) const SCAD_WARN_UNUSED_RESULT;

  Shape LinearExtrude(const LinearExtrudeParams& params) const
      SCAD_WARN_UNUSED_RESULT;

  Shape Color(double r,
              double g,
              double b,
              double a = 1.0) const SCAD_WARN_UNUSED_RESULT;
  Shape Alpha(double a) const SCAD_WARN_UNUSED_RESULT;

  Shape Subtract(const Shape& other) const SCAD_WARN_UNUSED_RESULT;
  Shape Add(const Shape& other) const SCAD_WARN_UNUSED_RESULT;

  Shape Scale(double x, double y, double z) const SCAD_WARN_UNUSED_RESULT;
  Shape Scale(double s) const SCAD_WARN_UNUSED_RESULT;

  Shape OffsetRadius(double r,
                     bool chamfer = false) const SCAD_WARN_UNUSED_RESULT;
  Shape OffsetDelta(double delta,
                    bool chamfer = false) const SCAD_WARN_UNUSED_RESULT;

  Shape Comment(const std::string& comment) const SCAD_WARN_UNUSED_RESULT;

 private:
  std::shared_ptr<const ScadWriter> scad_;
};

using Transform = std::function<Shape(const Shape&)>;

struct CubeParams {
  double x = 1;
  double y = 1;
  double z = 1;
  bool center = true;
};

Shape MakeCube(const CubeParams& params) SCAD_WARN_UNUSED_RESULT;
Shape MakeCube(double x, double y, double z, bool center = true)
    SCAD_WARN_UNUSED_RESULT;
Shape MakeCube(double size, bool center = true) SCAD_WARN_UNUSED_RESULT;

struct SphereParams {
  double r = 1;
  double fn = 0;
  double fa = 12;
  double fs = 2;
};

Shape MakeSphere(const SphereParams& params) SCAD_WARN_UNUSED_RESULT;
Shape MakeSphere(double radius) SCAD_WARN_UNUSED_RESULT;

struct CylinderParams {
  double h = 1;
  double r1 = 1;
  double r2 = 1;
  double fn = 0;
  bool center = true;
};

Shape MakeCylinder(const CylinderParams& params) SCAD_WARN_UNUSED_RESULT;
Shape MakeCylinder(double height,
                   double radius,
                   double fn = 0) SCAD_WARN_UNUSED_RESULT;

struct Point2d {
  double x = 0;
  double y = 0;
};

Shape MakePolygon2d(const std::vector<Point2d>& points) SCAD_WARN_UNUSED_RESULT;

Shape HullAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Hull(const Shape& shape, const Shapes&... more_shapes) {
  return HullAll({shape, more_shapes...});
}

Shape UnionAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Union(const Shape& shape, const Shapes&... more_shapes) {
  return UnionAll({shape, more_shapes...});
}

Shape DifferenceAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Difference(const Shape& shape, const Shapes&... more_shapes) {
  return DifferenceAll({shape, more_shapes...});
}

Shape IntersectionAll(const std::vector<Shape>& shapes) SCAD_WARN_UNUSED_RESULT;

template <typename... Shapes>
Shape Intersection(const Shape& shape, const Shapes&... more_shapes) {
  return IntersectionAll({shape, more_shapes...});
}

}  // namespace scad
