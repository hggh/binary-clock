$fn=190;

difference() {
    cube([7, 12, 9]);
    translate([3.5, 6, 0]) {
        cylinder(d=3.2, h=10);
        rotate([0, 0, 30])cylinder(r = 5.5 / 2 / cos(180 / 6) + 0.05, h=4, $fn=6);
    }
}