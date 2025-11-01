build: 
	g++ -o zmalloc -c zmalloc.cpp

clean:
	rm -rf zmalloc

test:
	g++ -I./ -o test test.cpp zmalloc.cpp
	./test
	rm -rf test