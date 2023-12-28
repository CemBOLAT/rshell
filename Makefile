NAME := myShell
SRCS := ./src/main.cpp \
		./src/Shell.cpp \
		./src/File.cpp \
		./src/Directory.cpp \
		./src/RegularFile.cpp \
		./src/LoadFile.cpp \
		./src/Utils.cpp \
		./src/Executor.cpp \
		./src/SaveFile.cpp \
		#./src/UtilsTemplate.cpp \

OBJS := ./obj/main.o \
		./obj/Shell.o \
		./obj/File.o \
		./obj/Directory.o \
		./obj/RegularFile.o \
		./obj/LoadFile.o \
		./obj/Utils.o \
		./obj/Executor.o \
		./obj/SaveFile.o \
		#./obj/UtilsTemplate.o \

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic -g

all : $(NAME)

$(NAME) : $(OBJS) $(Header)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJS) : ./obj/%.o : ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
