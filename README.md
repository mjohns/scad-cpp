# OpenSCAD++

A generator for creating OpenSCAD files in c++ using an object oriented approach.

Example usage:
```
Union(MakeCube(10),
      MakeSphere(5).Translate(0, 0, 5)).WriteToFile("example.scad);

g++ *.cc -std=c++11 && ./a.out
```
