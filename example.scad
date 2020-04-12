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
      cube(1);
    }
  }
  linear_extrude (height = 30.000, center = true, convexity = 10.000, twist = 0.000, slices = 20, scale = 1.000) {
    projection (cut = false) {
      cylinder(h = 1.000, r1 = 2.000, r2 = 2.000, center = true, $fn = 20.000);
    }
  }
}
