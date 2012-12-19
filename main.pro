CONFIG -= qt
QMAKE_CFLAGS += -g -O3
QMAKE_CXXFLAGS += -g -O3

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS -= -O1
QMAKE_LFLAGS += -O3
QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3
QMAKE_LDFLAGS -= -O1
QMAKE_LDFLAGS += -O3

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU
SOURCES += main.cpp Particle.cpp Actor.cpp Scene.cpp ParticleEmitter.cpp MinecraftScene.cpp FireActor.cpp util.cpp
HEADERS += Particle.h util.h Actor.h Scene.h ParticleEmitter.h MinecraftScene.h FireActor.h
