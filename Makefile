CC=g++
CFLAGS = -g -Wall `sdl2-config --cflags` --std=c++11
INCLUDES = -I/. -I/usr/local/include
LFLAGS =
LIBS = `sdl2-config --libs`
# STATE_SRC = $(addprefix State/, GameStateMachine.cpp MainMenuState.cpp \
# 	PauseState.cpp GameOverState.cpp PlayState.cpp)
# OBJECT_SRC = $(addprefix Object/, SDLGameObject.cpp MenuButton.cpp \
# 	GameObjectFactory.cpp Player.cpp Enemy.cpp AnimatedGraphic.cpp\
# 	BulletHandler.cpp ScrollingBackground.cpp)
# MAP_SRC = $(addprefix Map/, Level.cpp TileLayer.cpp LevelParser.cpp ObjectLayer.cpp)
# SRCS = Game.cpp main.cpp\
# 	TextureManager.cpp InputHandler.cpp SoundManager.cpp StateParser.cpp\
# 	CollisionManager.cpp SoundManager.cpp\
# 	base64.cpp

# SRCS += $(STATE_SRC) $(OBJECT_SRC) $(MAP_SRC)
SRCS = $(wildcard *.cpp)
STATE_SRCS = $(wildcard State/*.cpp)
OBJECT_SRCS = $(wildcard Object/*.cpp)

SRCS += $(STATE_SRCS) $(OBJECT_SRCS)
OBJS = $(SRCS:.cpp=.o)
MAIN = hebipal

.PHONY: all clean


all: $(MAIN)
	@echo Compiled $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(MAIN)
