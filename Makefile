default: still_good still_good.stl

still_good.stl: still_good.scad
	OpenSCAD -o still_good.stl still_good.scad

still_good: still_good.ino simulator.c display.c click_handler.c
	g++ simulator.c display.c click_handler.c -DSIM=1 -g -o still_good

clean:
	-rm -f still_good still_good.stl
