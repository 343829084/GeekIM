SRCDIR = ./
LINUXBIN_PATH = ../../bin



SRC = $(wildcard $(SRCDIR)*.cpp)

LIB_OBJS = $(patsubst %.cpp,%.o,$(SRC))


EXE_NAME = client

CC = gcc
CPP = g++
LD = g++

CFLAGS = -g   -w -O3 -D__linux__ -D_LINUX -D_TEST -fPIC 
DEFINES = -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_ISOC9X_SOURCE

INC := -I$(SRCDIR) 


LDLIBS =  -lcurses -lpthread

all: $(EXE_NAME)


$(EXE_NAME): $(LIB_OBJS) | $(LINUXBIN_PATH)
	@ echo "Create $@"
	$(LD)  $(CFLAGS) $(LIB_OBJS) -o $(LINUXBIN_PATH)/$(EXE_NAME) $(LDLIBDIR) $(LDLIBS)
	

%.o: %.S
	@ echo "Compiling $^"
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -c -o $@ $<
	
%.o:%.c
	@ echo "Compiling $^"
	$(CC) $(CFLAGS) $(INC) $(DEFINES) -c $< -o $@

%.o:%.cpp
	@ echo "Compiling $^"
	$(CPP) $(CFLAGS) $(INC) $(DEFINES) -c $< -o $@
	

clean:
	rm -f $(LIB_OBJS)


rebuild: clean all

install: 





