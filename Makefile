CXXFLAGS=-std=c++11 -I../PEGTL/include/tao -g -O
LXXFLAGS = -g

OBJS = classify.o Thing.o Explainer.o

all: classify

clean:
	rm -f $(OBJS) classify

classify.o: classify.cc grammar.h
	$(CXX) $(CXXFLAGS) -c classify.cc

Thing.o: Thing.cc Thing.h
	$(CXX) $(CXXFLAGS) -c Thing.cc

Explainer.o: Explainer.cc Explainer.h
	$(CXX) $(CXXFLAGS) -c Explainer.cc

classify: $(OBJS)
	$(CXX) $(OBJS) $(LXXFLAGS) -o classify


