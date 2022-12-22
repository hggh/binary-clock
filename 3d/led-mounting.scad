$fn=190;

module ws2812b() {
    translate([-0.1, -0.1, -1]) cube([5+0.2, 5+0.2, 10]);
}

difference() {
    union() {
        cube([129, 41, 1.2]);
        translate([20, 41, 0]) cube([109, 45, 1.2]);
        translate([22, -19, 0]) cube([8, 124, 1.2]);
        translate([99, -19, 0]) cube([8, 124, 1.2]);
    }
    
    translate([7.167, 7.667, 0]) ws2812b();
    translate([7.167, 29.667, 0]) ws2812b();
    
    translate([27.167, 7.167,0]) ws2812b();
    translate([27.167, 27.167, 0]) ws2812b();
    translate([27.167, 51.167, 0]) ws2812b();
    translate([27.167, 73.167, 0]) ws2812b();
    
    translate([52.167, 7.167,0]) ws2812b();
    translate([52.167, 27.167, 0]) ws2812b();
    translate([52.167, 51.167, 0]) ws2812b();

    translate([72.167, 7.167,0]) ws2812b();
    translate([72.167, 27.167, 0]) ws2812b();
    translate([72.167, 51.167, 0]) ws2812b();
    translate([72.167, 73.167, 0]) ws2812b();

    translate([97.167, 7.167,0]) ws2812b();
    translate([97.167, 27.167, 0]) ws2812b();
    translate([97.167, 51.167, 0]) ws2812b();

    translate([117.167, 7.167,0]) ws2812b();
    translate([117.167, 27.167, 0]) ws2812b();
    translate([117.167, 51.167, 0]) ws2812b();
    translate([117.167, 73.167, 0]) ws2812b();
    
    translate([26, -12.5, -1]) cylinder(d=3.2, h=10);
    translate([103, -12.5, -1]) cylinder(d=3.2, h=10);

    translate([26, 98.5, -1]) cylinder(d=3.2, h=10);
    translate([103, 98.5, -1]) cylinder(d=3.2, h=10);
}