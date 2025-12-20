all: csp_v1

csp_v1: csp.cpp ucodes.h ucodes.cpp ucenv.h
	g++ csp.cpp ucodes.cpp -o csp -std=c++11

csp_v2: csp.cpp ucodes.h ucodes.cpp
	g++ -I$$LUNA_HOME/include \
		-I$$LUNA_HOME/plugins/cpu_threadpool/global/include \
		-std=c++11 \
		csp.cpp ucodes.cpp \
		-L$$LUNA_HOME/lib \
		-lrts -lcpu_threadpool \
		-Wl,--no-as-needed \
		-o csp

run: csp
	LD_LIBRARY_PATH=$$LUNA_HOME/lib ./$<

.PHONY: all run

