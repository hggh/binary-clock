//button 12mm
// hoch = 4mm


difference() {
    cube([30, 10, 2  + 4]);
    translate([9, 0, 2]) cube([12, 10, 10]);
    
    translate([4.5, 5, 0]) {
        cylinder(d=3.2, h=8, $fn=190);
        cylinder(r = 5.5 / 2 / cos(180 / 6) + 0.05, h=3, $fn=6);
    }
    translate([25.5, 5, 0]) {
        cylinder(d=3.2, h=8, $fn=190);
        cylinder(r = 5.5 / 2 / cos(180 / 6) + 0.05, h=3, $fn=6);
    }
}
