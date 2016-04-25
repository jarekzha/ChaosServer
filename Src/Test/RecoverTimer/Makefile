
Obj=User.o main.o

all: test

%.o: %.cpp
	g++ -g -c $< -o $@ -I../../output/include

test: $(Obj)
	g++ $(Obj) -o $@ -L../../output/lib -lengine -lpthread -lboost_serialization

clean:
	rm -rf test *.o

