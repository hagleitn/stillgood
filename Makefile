default: still_good still_good.stl

still_good.stl: still_good.scad
	OpenSCAD -o still_good.stl still_good.scad

still_good: still_good.ino simulator.cpp display.cpp click_handler.cpp
	g++ simulator.cpp display.cpp click_handler.cpp -DSIM=1 -g -o still_good

clean:
	-rm -rf still_good still_good.stl still_good.dSYM
