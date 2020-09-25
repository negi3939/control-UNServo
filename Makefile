################################################################################################################################################
###        In this Makefile made by taka, you can use as                                                               	                     ###
###        $ make                     # You can get the executable file which written in TARGET. And the executable file will run.           ###
###        $ make target=hoge         # You can get the executable file which written in hoge. And the executable file will run.             ###
###        $ make argv=hoge           # You can get the executable file which written in TARGET. And the executable file will run with argv. ###
###        $ make notrun=1            # You can get the executable file which written in TARGET. The executable file will not run.		     ###
###        $ make clean               # The executable file which written in TARGET will removed.                                            ###
###        $ make clean target=hoge   # The executable file which written in hoge will removed.                                              ###
################################################################################################################################################

ifdef target
	TARGET=$(target)
else
	TARGET=control
	#key
	#control
	#serial
	#file
endif

ifeq ($(TARGET),key)
	SOURCE_MAIN = keyboard.cpp
	CXXFLAGS = -DKEY_IS_MAIN
endif

ifeq ($(TARGET),serial)
	SOURCE_MAIN = serial.cpp
	SOURCE_SUB = keyboard.cpp
	CXXFLAGS = -DSERIAL_IS_MAIN
endif

ifeq ($(TARGET),file)
	SOURCE_MAIN = filesave.cpp
	CXXFLAGS = -DFILE_IS_MAIN
endif

ifeq ($(TARGET),control)
	SOURCE_MAIN = contrServo.cpp
	SOURCE_SUB = keyboard.cpp serial.cpp filesave.cpp
	CXXFLAGS = -DCONTROL_IS_MAIN
endif


ifdef argv
	COMMAND = echo run;./$(SOURCE_MAIN:%.cpp=%.out) argv
else
	COMMAND = echo run;./$(SOURCE_MAIN:%.cpp=%.out)
endif
ifdef notrun
		COMMAND = echo You got $(SOURCE_MAIN:%.cpp=%.out).
endif

PROGRAM = $(SOURCE_MAIN:%.cpp=%.exe)
SUBOBJ = $(SOURCE_SUB:%.cpp=%.o)

DIRX = /usr/X11R6/lib

CXXFLAGS += -fpermissive
LDFLAGS	 = -L "$(DIRX)" -lm

all: $(PROGRAM)

%.out: %.o $(SUBOBJ)
	g++ -o $@ $^ $(LDFLAGS) -w
	#$(COMMAND)
%.exe: %.o $(SUBOBJ)
	g++ -o $@ $^ $(LDFLAGS) -w
	#$(COMMAND)
%.o : %.cpp
	g++ -o $@ $< -c $(CXXFLAGS) -w
clean:
	rm -f *.o $(PROGRAM)
