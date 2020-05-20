CC=g++

rtext: rtext.o qstring.o parser.tab.o lexer.o

rtext.o: rtext.cpp parser.tab.hpp

qstring.o: qstring.cpp

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d parser.ypp
	
parser.tab.o:parser.tab.cpp

lexer.o:lexer.cpp

lexer.cpp:lexer.l parser.tab.hpp
	flex lexer.l
	mv lex.yy.c lexer.cpp
