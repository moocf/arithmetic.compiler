build:
	@flex -o main.yy.cpp main.l
	@bison -o main.tab.cpp -d main.y
	@g++ main.cpp main.tab.cpp main.yy.cpp -ll

clean:
	@rm -f a.out main.tab.cpp main.tab.hpp main.yy.cpp
