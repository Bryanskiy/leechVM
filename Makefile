.PHONY: all

all:
	$(MAKE) -C frontend
	g++ driver/driver.cc frontend/parser.tab.cc frontend/lex.yy.cc main.cc -o main.out