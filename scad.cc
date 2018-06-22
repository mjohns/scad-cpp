#include "scad.h"

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

namespace scad {
namespace {

const int kTabSize = 2;

const char* BoolStr(bool b) {
  return b ? "true" : "false";
}

void WriteIndent(std::FILE* file, int indent_level) {
  for (int i = 0; i < indent_level * kTabSize; ++i) {
    fputc(' ', file);
  }
}

void WriteComposite(std::FILE* file,
                    const std::function<void(std::FILE*)>& write_name,
                    const std::vector<Shape>& shapes,
                    int indent_level) {
  WriteIndent(file, indent_level);
  write_name(file);
  fprintf(file, " {\n");
  for (const Shape& s : shapes) {
    s.AppendScad(file, indent_level + 1);
  }
  WriteIndent(file, indent_level);
  fprintf(file, "}\n");
}

Shape MakeComposite(const std::function<void(std::FILE*)>& write_name,
                    const std::vector<Shape>& shapes) {
  return Shape(
      std::make_shared<ScadWriter>([=](std::FILE* file, int indent_level) {
        WriteComposite(file, write_name, shapes, indent_level);
      }));
}

Shape MakeLiteralComposite(const char* name, const std::vector<Shape>& shapes) {
  return Shape(
      std::make_shared<ScadWriter>([=](std::FILE* file, int indent_level) {
        WriteComposite(file,
                       [=](std::FILE*) { fprintf(file, "%s", name); },
                       shapes,
                       indent_level);
      }));
}

Shape MakePrimitive(const std::function<void(std::FILE*)>& scad_writer) {
  return Shape(
      std::make_shared<ScadWriter>([=](std::FILE* file, int indent_level) {
        WriteIndent(file, indent_level);
        scad_writer(file);
        fprintf(file, "\n");
      }));
}

}  // namespace

Shape MakeCube(const CubeParams& params) {
  return MakePrimitive([=](std::FILE* file) {
    fprintf(file,
            "cube(size = [%.3f, %.3f, %.3f], center = %s);",
            params.x,
            params.y,
            params.z,
            BoolStr(params.center));
  });
}

Shape MakeCube(double x, double y, double z, bool center) {
  CubeParams params;
  params.x = x;
  params.y = y;
  params.z = z;
  params.center = center;
  return MakeCube(params);
}

Shape MakeCube(double size, bool center) {
  return MakeCube(size, size, size, center);
}

Shape MakeSphere(const SphereParams& params) {
  return MakePrimitive([=](std::FILE* file) {
    fprintf(file,
            "sphere($fs = %.3f, $fn = %.3f, $fa = %.3f, r = %.3f);",
            params.fs,
            params.fn,
            params.fa,
            params.r);
  });
}

Shape MakeSphere(double radius) {
  SphereParams params;
  params.r = radius;
  return MakeSphere(params);
}

Shape MakeCylinder(const CylinderParams& params) {
  return MakePrimitive([=](std::FILE* file) {
    fprintf(
        file,
        "cylinder(h = %.3f, r1 = %.3f, r2 = %.3f, $fn = %.3f, center = %s);",
        params.h,
        params.r1,
        params.r2,
        params.fn,
        BoolStr(params.center));
  });
}

Shape MakeCylinder(double height, double radius, double fn) {
  CylinderParams params;
  params.h = height;
  params.r1 = radius;
  params.r2 = radius;
  params.fn = fn;
  return MakeCylinder(params);
}

Shape MakePolygon2d(const std::vector<Point2d>& points) {
  return MakePrimitive([=](std::FILE* file) {
    fprintf(file, "polygon(points = [");
    for (int i = 0; i < points.size(); ++i) {
      const Point2d& p = points[i];
      if (i != 0) {
        fputc(',', file);
      }
      fprintf(file, "[%.3f, %.3f],", p.x, p.y);
    }
    fprintf(file, "]);");
  });
}

Shape HullAll(const std::vector<Shape>& shapes) {
  return MakeLiteralComposite("hull ()", shapes);
}

Shape UnionAll(const std::vector<Shape>& shapes) {
  return MakeLiteralComposite("union ()", shapes);
}

Shape DifferenceAll(const std::vector<Shape>& shapes) {
  return MakeLiteralComposite("difference ()", shapes);
}

Shape IntersectionAll(const std::vector<Shape>& shapes) {
  return MakeLiteralComposite("intersection ()", shapes);
}

Shape Shape::Translate(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "translate ([%.3f, %.3f, %.3f])", x, y, z);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Mirror(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "mirror ([%.3f, %.3f, %.3f])", x, y, z);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Rotate(double degrees, double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(
        file, "rotate (a = %.3f, v = [%.3f, %.3f, %.3f])", degrees, x, y, z);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::RotateX(double degrees) const {
  return Rotate(degrees, 1, 0, 0);
}

Shape Shape::RotateY(double degrees) const {
  return Rotate(degrees, 0, 1, 0);
}

Shape Shape::RotateZ(double degrees) const {
  return Rotate(degrees, 0, 0, 1);
}

Shape Shape::LinearExtrude(const LinearExtrudeParams& params) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file,
            "linear_extrude (height = %.3f, center = %s, convexity = %.3f, "
            "twist = %.3f, slices = %d, scale = %.3f)",
            params.height,
            BoolStr(params.center),
            params.convexity,
            params.twist,
            params.slices,
            params.scale);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Color(double r, double g, double b, double a) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "color (c = [%.3f, %.3f, %.3f, %.3f])", r, g, b, a);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Alpha(double a) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "color (alpha = %.3f)", a);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Scale(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "scale ([%.3f, %.3f, %.3f])", x, y, z);
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Scale(double s) const {
  return Scale(s, s, s);
}

Shape Shape::OffsetRadius(double r, bool chamfer) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "offset (r = %.3f, chamfer = %s)", r, BoolStr(chamfer));
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::OffsetDelta(double delta, bool chamfer) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(
        file, "offset (delta = %.3f, chamfer = %s)", delta, BoolStr(chamfer));
  };
  return MakeComposite(write_name, {*this});
}

Shape Shape::Subtract(const Shape& other) const {
  return Difference(*this, other);
}

Shape Shape::Add(const Shape& other) const {
  return Union(*this, other);
}

Shape Shape::Comment(const std::string& comment) const {
  Shape shape_copy = *this;
  return Shape(
      std::make_shared<ScadWriter>([=](std::FILE* file, int indent_level) {
        WriteIndent(file, indent_level);
        fprintf(file, "/* %s */\n", comment.c_str());
        shape_copy.AppendScad(file, indent_level);
      }));
}

void Shape::AppendScad(std::FILE* file, int indent_level) const {
  if (!scad_) {
    return;
  }
  (*scad_)(file, indent_level);
}

void Shape::WriteToFile(const std::string& file_name) const {
  std::FILE* file = std::fopen(file_name.c_str(), "w");
  if (file == nullptr) {
    fprintf(stderr, "Could not open file %s\n", file_name.c_str());
    return;
  }
  AppendScad(file, 0);
  std::fclose(file);
}

}  // namespace scad
