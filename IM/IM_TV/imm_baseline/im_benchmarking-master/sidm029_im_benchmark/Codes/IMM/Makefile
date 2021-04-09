all: imm_continuous imm_discrete

imm_continuous: src/*.cpp src/*.h
	g++ -DCONTINUOUS src/imm.cpp -Wall -std=c++0x -O3 src/sfmt/SFMT.c  -o imm_continuous

imm_discrete: src/*.cpp src/*.h
	g++ -DDISCRETE src/imm.cpp -Wall -std=c++0x -O3 src/sfmt/SFMT.c  -o imm_discrete

imm: src/*.cpp src/*.h
	g++ src/imm.cpp -Wall -std=c++0x -O3 src/sfmt/SFMT.c  -o imm
O0:
	g++ src/imm.cpp -Wall -std=c++0x -O0 src/sfmt/SFMT.c  -o imm
O1:
	g++ src/imm.cpp -Wall -std=c++0x -O1 src/sfmt/SFMT.c  -o imm
O2:
	g++ src/imm.cpp -Wall -std=c++0x -O2 src/sfmt/SFMT.c  -o imm
profile:
	g++ src/imm.cpp -Wall -std=c++0x -O2 src/sfmt/SFMT.c  -pg -o imm.profile

cont_profile:
	g++ -DCONTINUOUS src/imm.cpp -Wall -std=c++0x -O3 src/sfmt/SFMT.c  -o imm_continuous -pg -o imm_continuous.profile

clean:
	rm -f *.o *~ imm_continuous imm_discrete
