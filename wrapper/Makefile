SRC_LIB_PATH = ../simple_caro/src

SRC_PATH = ./Simple_Caro_wrapper.cpp \
			$(SRC_LIB_PATH)/board_context.cpp \
			$(SRC_LIB_PATH)/player_context.cpp \
			$(SRC_LIB_PATH)/game_judge.cpp \
			$(SRC_LIB_PATH)/ruling.cpp \
			$(SRC_LIB_PATH)/utils.cpp \
			$(SRC_LIB_PATH)/simple_caro.cpp

LIB_NAME = libSimple_Caro.a

OBJECTS = $(SRC_PATH:.cpp=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm -f $(OBJECTS) $(LIB_NAME)

.PHONY: all clean