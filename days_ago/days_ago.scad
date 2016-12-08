$fa = 1;
$fs = 0.5;
$fn = 0;

wall = 2;
dims = [20, 35, 12];
cutout = [15.5, 14.5, 2*wall];
corner_r = 3;
offset = 3;
lip = 1;
dist = 10;

latch = [wall-1, 7, dims[2]-wall-1];

button = [6.5,6.5,2*wall];
offset_button = 3;

module skin(debug=false, thickness=5, box=400) {
    if (!debug) intersection() { 
        children(); 
        minkowski() {
            cube(2*thickness,center=true);
            difference() { 
                cube(box,center=true); 
                children(); 
            } 
        } 
    }
    
    if (debug) children();
}

module enclosure(thickness=10) {
    difference() {
        skin(thickness=thickness) {
            hull() {
                translate([0,0,0]) cylinder(h=dims[2], r=corner_r, center=true);
                translate([dims[0],0,0]) cylinder(h=dims[2], r=corner_r, center=true);
                translate([0,dims[1],0]) cylinder(h=dims[2], r=corner_r, center=true);
                translate([dims[0],dims[1],0]) cylinder(h=dims[2], r=corner_r, center=true);
            }
        }
        translate([dims[0]/2-cutout[0]/2, dims[1]-cutout[1]-offset, dims[2]/2-cutout[2]/2]) 
            cube(cutout);
        translate([dims[0]/2-button[0]/2, offset_button, dims[2]/2-button[2]/2])
            cube(button);
    }
}

module latch__() {
    latch_();
    translate([dims[0],0,0]) mirror([1,0,0]) latch_();
}

module latch_(solid=false) {
    translate([-corner_r+1, dims[1]/2-latch[1]/2, -dims[2]/2+wall]) cube(latch);
    translate([-corner_r, dims[1]/2-latch[1]/2, -dims[2]/2+wall]) 
        cube([latch[0], latch[1], wall+1]);
}

module bottom() {
    difference() {
        enclosure(thickness=wall);
        translate([-dims[0], -dims[1], 0]) cube([4*dims[0], 4*dims[1], dims[2]]);
        latch__();
    }
}

module top() {
    difference() {
        enclosure(thickness=wall);
        translate([-dims[0], -dims[1], -dims[2]]) cube([4*dims[0], 4*dims[1], dims[2]]);
    }
    latch__();
}

translate([-dims[0]/2-dist, -dims[1]/2, 0]) bottom();
translate([dims[0]/2+dist, dims[1]/2, 0]) rotate([180,0,0]) top();
