default: still_good still_good.stl

still_good.stl: still_good.scad
	OpenSCAD -o still_good.stl still_good.scad

still_good: still_good.ino Simulator.cpp Display.cpp ClickHandler.cpp Timeline.cpp
	g++ Simulator.cpp Display.cpp ClickHandler.cpp Timeline.cpp -DSIM=1 -g -o still_good

clean:
	-rm -rf still_good still_good.stl still_good.dSYM
