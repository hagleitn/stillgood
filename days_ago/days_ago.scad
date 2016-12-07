$fa = 2;
$fs = 1;
$fn = 0;

wall = 3;
dims = [20, 10, 10];
cutout = [18, 6, 2*wall];
corner_r = 3;
offset = 3;
lip = 2;

button = [2,2,2*wall];
offset_button = 0;

module skin(debug=false, thickness=5, box=400) {
    if (!debug) intersection() { 
        children(); 
        minkowski() {
            cube(thickness,center=true);
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
        translate([dims[0]/2-cutout[0]/2, dims[1]-cutout[1]/2-offset, dims[2]/2-cutout[2]/2]) 
            cube(cutout);
        translate([dims[0]/2-button[0]/2, offset_button, dims[2]/2-button[2]/2])
            cube(button);
    }
}

module bottom() {
    union() {
        difference() {
            enclosure(wall);
            translate([-dims[0], -dims[1], -lip/2]) cube([4*dims[0], 4*dims[1], dims[2]]);
        }
        difference() {
            enclosure(wall/2);
            translate([-dims[0], -dims[1], lip/2]) cube([4*dims[0], 4*dims[1], dims[2]]);
        }
    }
}

module top() {
    union() {
        difference() {
            enclosure(wall);
            translate([-dims[0], -dims[1], -dims[2]+lip/2]) cube([4*dims[0], 4*dims[1], dims[2]]);
        }
        difference() {
            difference() {
                enclosure(wall);
                enclosure(wall/2);
            }
            translate([-dims[0], -dims[1], -dims[2]-lip/2]) cube([4*dims[0], 4*dims[1], dims[2]]);
        }
    }
}

translate([-dims[0]/2-offset, 0, 0]) bottom();
translate([dims[0]/2+2*offset, 0, 0]) rotate([180,0,0]) top();
