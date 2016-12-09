default: still_good still_good.stl

still_good.stl: still_good.scad
	OpenSCAD -o still_good.stl still_good.scad

still_good: still_good.ino simulator.c
	g++ simulator.c -DSIM=1 -o still_good

clean:
	-rm -f still_good still_good.stl
