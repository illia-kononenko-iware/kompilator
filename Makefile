build: lex parse code_generator.o operations.o conditions.o flow_control.o command.o var_map.o
	g++ parser.tab.cpp lex.yy.c -g3 -o compiler -lfl code_generator.o operations.o conditions.o flow_control.o command.o var_map.o
parse:
	bison -vd parser.ypp
lex:
	flex lexer.l
clean:
	rm lex.yy.c parser.output parser.tab.c parser.tab.h

code_generator.o: classes/code_generator.cpp classes/code_generator.h
	g++ -g3 -c classes/code_generator.cpp -o code_generator.o

operations.o: classes/operations.cpp classes/code_generator.h
	g++ -g3 -c classes/operations.cpp -o operations.o

conditions.o: classes/conditions.cpp classes/code_generator.h
	g++ -g3 -c classes/conditions.cpp -o conditions.o

flow_control.o: classes/flow_control.cpp classes/code_generator.h
	g++ -g3 -c classes/flow_control.cpp -o flow_control.o

command.o: classes/command.cpp classes/command.h
	g++ -g3 -c classes/command.cpp -o command.o

var_map.o: classes/var_map.cpp classes/var_map.h
	g++ -g3 -c classes/var_map.cpp -o var_map.o