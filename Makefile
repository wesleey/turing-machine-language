all: bin/tml

src/lexer/lex.yy.c src/parser/y.tab.c include/y.tab.h:
	yacc -d -o src/parser/y.tab.c src/parser/parser.y
	mv src/parser/y.tab.h include
	flex -o src/lexer/lex.yy.c src/lexer/lexer.l

bin/tml: src/lexer/lex.yy.c src/parser/y.tab.c include/y.tab.h
	mkdir -p bin
	gcc -o bin/tml -Iinclude \
		src/lexer/lex.yy.c src/parser/y.tab.c src/semantic/semantic.c\
		src/core/ast.c src/core/table.c src/core/tree.c src/core/queue.c src/core/machine.c src/core/error.c \
		src/main/main.c -lfl

clean:
	rm -rf bin
	rm -f src/lexer/lex.yy.c src/parser/y.tab.c include/y.tab.h
