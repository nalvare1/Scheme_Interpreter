CMP = g++ -std=c++11
MAIN = uscheme
CLASS = node
SHELL = bash
EXEC = scheme

$(EXEC): $(MAIN).o $(CLASS).o
	$(CMP) $(MAIN).o $(CLASS).o -o $(EXEC)

$(MAIN).o: $(MAIN).cpp $(CLASS).h
	$(CMP) -c $(MAIN).cpp -o $(MAIN).o

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CMP) -c $(CLASS).cpp -o $(CLASS).o
	
test: test-scheme

test-scheme: scheme
	@echo Testing Scheme Interpreter...
	@diff --suppress-common-lines -y <(./scheme < input) output

clean:
	rm *.o
	rm $(EXEC)
