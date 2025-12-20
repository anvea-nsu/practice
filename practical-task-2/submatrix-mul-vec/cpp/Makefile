all: cmv_v1

cmv_v1: cmv.cpp ucodes.h ucodes.cpp ucenv.h
	g++ cmv.cpp ucodes.cpp -o cmv -std=c++11

cmv_v2: cmv.cpp ucodes.h ucodes.cpp
	g++ -I$$LUNA_HOME/include \
		-I$$LUNA_HOME/plugins/cpu_threadpool/global/include \
		-std=c++11 \
		cmv.cpp ucodes.cpp \
		-L$$LUNA_HOME/lib \
		-lrts -lcpu_threadpool \
		-Wl,--no-as-needed \
		-o csp

run: cmv
	LD_LIBRARY_PATH=$$LUNA_HOME/lib ./$<

.PHONY: all run

