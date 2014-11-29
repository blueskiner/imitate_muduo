
BIN_DIR=.

SRC_DIR=.

# source files
#SRVSRCS=$(wildcard ./*.cpp $(SRC_DIR)/*.cpp)

EXCLUDE_FILES=./client.cpp

########## do not change below ##########
EXTS := *.cpp
DIRS := ${shell find ${SRC_DIR} -type d -print}
SRCS := $(foreach dir, $(DIRS), $(wildcard $(addprefix $(dir)/, $(EXTS))))
SRCS := $(filter-out $(EXCLUDE_FILES), $(SRCS))
OBJS=$(patsubst %.cpp, %.o, $(SRCS))

# Compile Flags
CXXFLAGS=-I. -Wall -DDEBUG -g -ansi

# Link Flags
LDFLAGS=

SERVER=$(BIN_DIR)/server

###########################################################

#Final Target!!!
all:$(SERVER)

#Target Depends
$(SERVER):$(filter-out $(EXCLUDE_OBJS), $(OBJS)) $(PROOBJS)
	@echo "Linking Target..." "########"$^"########"
	$(CXX) -o $@ $^ $(LDFLAGS)

#cpp object compiling
$(OBJS):%.o:%.cpp
	@echo "Compiling..." "########"$<"########"
	$(CXX) -c $< $(CXXFLAGS) -o $@
	@echo

#cc object compiling
#$(PROOBJS):%.o:%.cc
#	@echo "Compiling..." "########"$<"########"
#	$(CXX) -c $< $(CXXFLAGS) -o $@
#	@echo

#Cleaner
.PHONY:clean
clean:
	@echo
	@echo "Cleaning ..."
	$(RM) $(OBJS) "$(SERVER)"
	@echo
