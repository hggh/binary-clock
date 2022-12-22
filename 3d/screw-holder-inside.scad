
difference() {
    translate([-8.5/2, - 8.5/2, 0]) cube([8.5, 8.5, 7]);
    cylinder(r = 5.5 / 2 / cos(180 / 6) + 0.05, h=4, $fn=6);
    translate([0, 0, -5]) cylinder(d=3.2, h=20, $fn=190);
}