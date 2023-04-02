CPPFLAGS=-O2
CC=g++

rtext: rtext.o qstring.o parser.tab.o lexer.o

rtext.o: rtext.cpp parser.tab.hpp lexer.h parser.h qstring.h

qstring.o: qstring.cpp qstring.h

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d parser.ypp
	
parser.tab.o:parser.tab.cpp parser.h qstring.h

lexer.o:lexer.cpp

lexer.cpp lexer.h:lexer.l parser.tab.hpp
	flex --header-file=lexer.h lexer.l
	mv lex.yy.c lexer.cpp
