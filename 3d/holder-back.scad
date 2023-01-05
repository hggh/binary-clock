$fn=190;

difference() {
    union() {
        rotate([270, 0, 0]) cylinder(d=7, h=30);
        translate([0, 0, -3.5]) cube([115 - 2, 30, 2]);
    }
    rotate([270, 0, 0]) cylinder(d=3.2, h=30);
    
    translate([-10, 5-0.5, -5]) cube([16, 7+1, 10]);
    translate([-10, 30 - 8 -5 +0.5, -5]) cube([16, 7+1, 10]);
}

translate([115-2, 0, -3.5]) rotate([0, 0, 45])cube([21.21,21.21, 2]);



translate([60, -20, 0]) {
    cylinder(d=3, h=14 +2);
    cylinder(d=7, h=2);
}


translate([-5, 0, -2.5]) {
    difference() {
        union() {
            translate([-2, 5, 0]) cube([10.4, 7, 3]);
            translate([5, 5, 4]) rotate([270, 0, 0]) cylinder(d=7, h=7);
            translate([5, 30 - 5 -7, 4]) rotate([270, 0, 0]) cylinder(d=7, h=7);
            translate([-2, 30 - 5 -7, 0]) cube([10.4, 7, 3]);
            
        }
        translate([5, 5, 4]) rotate([270, 0, 0]) cylinder(d=3.2, h=7);
        translate([5, 30 - 5 -7, 4]) rotate([270, 0, 0]) cylinder(d=3.2, h=7);
        
        
    }
    translate([2, 5, 2]) {
        rotate([0, -65, 0]) cube([13, 7, 2]);
        translate([0, 13, 0])
        rotate([0, -65, 0]) cube([13, 7, 2]);
    }
    translate([-7, 5, 0]) {
        difference() {
            cube([5, 20, 3]);
            // Montage M3
            translate([2.5, 3.5, 0]) cylinder(d=3.2, h=10);
            translate([2.5, 3.5 + 7 +5+1, 0]) cylinder(d=3.2, h=10);
        }
    }

}
