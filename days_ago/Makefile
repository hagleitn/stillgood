default: days_ago stl

stl: days_ago.scad
	OpenSCAD -o days_ago.stl days_ago.scad

days_ago: days_ago.ino simulator.c
	gcc simulator.c -DSIM=1 -o days_ago

clean:
	-rm -f days_ago days_ago.stl
