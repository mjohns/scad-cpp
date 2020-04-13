difference () {
  union () {
    translate ([0.000, 0.000, 0.000]) {
      union () {
        cube (size = [ 10.000, 10.000, 10.000], center = true);
        translate ([0.000, 0.000, 5.000]) {
          sphere (r = 5.000);
        }
      }
    }
    translate ([15.000, 0.000, 0.000]) {
      union () {
        cube (size = [ 10.000, 10.000, 10.000], center = true);
        translate ([0.000, 0.000, 5.000]) {
          sphere (r = 5.000);
        }
      }
    }
    translate ([30.000, 0.000, 0.000]) {
      union () {
        cube (size = [ 10.000, 10.000, 10.000], center = true);
        translate ([0.000, 0.000, 5.000]) {
          sphere (r = 5.000);
        }
      }
    }
    translate ([45.000, 0.000, 0.000]) {
      cube(2);
    }
    translate ([0.000, -25.000, 0.000]) {
      polyhedron (points = [[10.000, 10.000, 0.000],[10.000, -10.000, 0.000],[-10.000, -10.000, 0.000],[-10.000, 10.000, 0.000],[0.000, 0.000, 10.000]], faces = [[0,1,4],[1,2,4],[2,3,4],[3,0,4],[1,0,3],[2,1,3]], convexity = 1);
    }
    translate ([20.000, -25.000, 0.000]) {
      linear_extrude (height = 10.000, center = true, convexity = 10.000, twist = 0.000, slices = 20, scale = 1.000) {
        polygon (points = [[0.000, 3.000],[2.598, 1.500],[2.598, -1.500],[0.000, -3.000],[-2.598, -1.500],[-2.598, 1.500]]);
      }
    }
  }
  linear_extrude (height = 30.000, center = true, convexity = 10.000, twist = 0.000, slices = 20, scale = 1.000) {
    projection (cut = false) {
      cylinder(h = 1.000, r1 = 2.000, r2 = 2.000, center = true, $fn = 20.000);
    }
  }
}
