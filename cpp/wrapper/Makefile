SRC_PATH = ./Simple_Caro_wrapper.cpp
LIB_NAME = libSimple_Caro.a

OBJECTS = $(SRC_PATH:.cpp=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(OBJECTS)
	ar rcs $@ $^

clean:
	rm -f $(OBJECTS) $(LIB_NAME)

.PHONY: all clean