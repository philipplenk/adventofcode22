CFLAGS				=	-Wfatal-errors -std=c++17 -Wall -pedantic -Werror -O3

INCLUDE_PATH			=	-I../common
LIBS				=	-lpthread -ltbb

all: 01 02
	
%: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE_PATH) $< -o $@ $(LIBS)
	
clean:
	rm -f 01 02
	
run1: 01
	cat input | ./01
	
run2: 02
	cat input | ./02
	
submit1: 01
	submit_result.sh 1 $(shell cat input | ./01)

submit2: 02
	submit_result.sh 2 $(shell cat input | ./02)

.PHONY: clean run1 run2 submit1 submit2
