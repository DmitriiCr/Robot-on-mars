CC=clang -Wall

PROGRAMMES=test_terrain test_robot robot_terrain curiosity curiosity-obs curiosity_perf curiosity-test

all: $(PROGRAMMES)

######################################################################
#                       Règles de compilation                        #
######################################################################

%.o: %.c
	$(CC) -c $<
test.o : test.c environnement.h programme.h interprete.h

test_terrain.o: test_terrain.c terrain.h

test_robot.o: test_robot.c robot.h

robot_terrain.o: robot_terrain.c terrain.h robot.h

robot.o: robot.c robot.h

terrain.o: terrain.c terrain.h

environnement.o: environnement.c environnement.h robot.h terrain.h observateur.h

programme.o: programme.c programme.h type_pile.h

interprete.o: interprete.c interprete.h environnement.h \
	programme.h type_pile.h robot.h terrain.h

interprete%.o: interprete%.c interprete.h environnement.h \
	programme.h type_pile.h robot.h terrain.h

type_pile.o: type_pile.c type_pile.h

curiosity.o: curiosity.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

curiosity-test.o: curiosity.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h observateur.h

curiosity-test%.o: curiosity.c environnement.h programme.h \
	interprete%.h robot.h terrain.h type_pile.h observateur.h

test_generation_terrains.o : test_generation_terrains.c terrain.h

generation_terrains.o : generation_terrains.c generation_terrains.h\
	terrain.h

 curiosity_perf.o : curiosity_perf.c programme.h environnement.h \
	terrain.h generation_terrains.h

observateur.o : observateur.c observateur.h


curiosity-obs.o: curiosity-obs.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

######################################################################
#                       Règles d'édition de liens                    #
######################################################################
curiosity_perf : curiosity_perf.o  programme.o interprete.o terrain.o generation_terrains.o\
	type_pile.o environnement.o robot.o observateur.o
	$(CC) $^ -o $@

test_generation_terrains : test_generation_terrains.o generation_terrains.o terrain.o
	$(CC) $^ -o $@

test_terrain: test_terrain.o terrain.o
	$(CC) $^ -o $@

test_robot: test_robot.o robot.o
	$(CC) $^ -o $@

robot_terrain: robot_terrain.o terrain.o robot.o
	$(CC) $^ -o $@

curiosity: curiosity.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o observateur.o
	$(CC) $^ -o $@

curiosity-obs: curiosity-obs.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o observateur.o
	$(CC) $^ -o $@

curiosity-test: curiosity-test.o environnement.o programme.o interprete.o \
	robot.o terrain.o type_pile.o observateur.o
	$(CC) $^ -o $@

curiosity-test%: curiosity-test.o environnement.o programme.o interprete%.o \
	robot.o terrain.o type_pile.o observateur.o
	$(CC) $^ -o $@

test : test.o environnement.o interprete.o programme.o \
	robot.o terrain.o type_pile.o observateur.o
	$(CC) $^ -o $@
clean:
	rm -f $(PROGRAMMES) *.o
