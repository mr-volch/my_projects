main_proc: main_proc.o xml_worker.o
	g++ main_proc.o xml_worker.o -lm -lxerces-c -o solution
	
dz_4_1.o: main_proc.cpp xml_worker.h
	g++ main_proc.cpp -c -Wall -Werror -g -lm -lxerces-c      
	

xml_worker.o: xml_worker.cpp xml_worker.h
	g++ xml_worker.cpp -c -Wall -Werror -g -lm -lxerces-c   
	
drawer: drawer.cpp
	g++ drawer.cpp -o picture -lsfml-graphics -lsfml-window -lsfml-system
	
clean:
	rm -f solution picture main_proc.o xml_worker.o 
