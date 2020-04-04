# OpenSCAD++

c++ code for easily generating OpenSCAD files.

Example usage:
```
Union(Cube(10), Sphere(5).Translate(0, 0, 5)).WriteToFile("example.scad");

g++ example.cc scad.cc -std=c++17 && ./a.out
```

Producing:
```
union () {
  cube (size = [ 10.000, 10.000, 10.000], center = true);
  translate ([0.000, 0.000, 5.000]) {
    sphere (r = 5.000);
  }
}
```
