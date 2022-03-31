#include <iostream>
#include <fstream>

#include <map>
#include <vector>

#include <cmath>

#include <algorithm>

#include <random>
#include <ctime>

using namespace std;

#define WEIGHT_TYPE size_t
#define GRAPH_TYPE map<Vertex*, vector<Vertex*>>

#define PROBABILITY -1;
#define ENDTEMPERATURE 0.0001;
#define TEMPERATURE 100;

#define random(a, b) (a + (rand() % b))
#define doubleRand(a, b) (double)(rand() / (double) RAND_MAX * (b - a) + a);


class Edge {	// graph edge

	size_t indexFrom, indexTo;	// edge indexes
	WEIGHT_TYPE weight;

public:
	Edge(size_t indexFrom, size_t indexTo, WEIGHT_TYPE weight) {
		this->indexFrom = indexFrom;
		this->indexTo = indexTo;
		this->weight = weight;
	}

	WEIGHT_TYPE getWeight() const {
		return this->weight;
	}

	size_t getIndexFrom() const { return this->indexFrom; }
	size_t getIndexTo() const { return this->indexTo; }
};

class Vertex {

	size_t selfIndex;

public:
	size_t getIndex() const {
		return selfIndex;
	}
	Vertex(size_t index) {
		this->selfIndex = index;
	}
};

class Graph {

	GRAPH_TYPE self;
	vector<Edge*> edges;	// graph edges
	vector<Vertex*> S = vector<Vertex*>();
	vector<Vertex*> vertexes = vector<Vertex*>();
	double temperature = TEMPERATURE;
	double endTemperature = ENDTEMPERATURE;
	double probability = PROBABILITY;

public:
	Edge* getEdgeByVertexes(const Vertex* v1, const Vertex* v2) {
		return this->getEdgeByIndexes(v1->getIndex(), v2->getIndex());
	}

	Edge* getEdgeByIndexes(size_t from, size_t to) {
		for (Edge* e : edges) {
			if (e->getIndexFrom() == from && e->getIndexTo() == to) {
				return e;
			}
		}
		return *edges.end();
	}

	Vertex* getVertexBySelfIndex(size_t index) {
		Vertex* v = new Vertex(index);
		for (GRAPH_TYPE::iterator it = this->self.begin(); it != this->self.end(); it++) {
			if ((*it).first->getIndex() == index) {
				return ((*it).first);
			}
		}
		return v;
	}

	void setVertexes()
	{
		for (GRAPH_TYPE::iterator it = this->self.begin(); it != this->self.end(); it++)
			vertexes.insert(vertexes.end(), (*it).second.begin(), (*it).second.end());	// iterate through graph, get all vertexes

		sort(vertexes.begin(), vertexes.end(), [](Vertex* v1, Vertex* v2) { return v1->getIndex() < v2->getIndex(); });	// get unique vertexes: 1, 2, 3 etc
		vertexes.erase(unique(vertexes.begin(), vertexes.end()), vertexes.end());
	}

	void setStartS() {	// start S (path)
		size_t currentIndex = random(1, vertexes.size());	// random start of the path
		vector<Vertex*> temp = vertexes;

		temp.erase(remove(temp.begin(), temp.end(), getVertexBySelfIndex(currentIndex)), temp.end());	// remove start vertex from temp vector

		auto rng = default_random_engine{};	// shuffle temp
		shuffle(begin(temp), end(temp), rng);
		S.push_back(getVertexBySelfIndex(currentIndex));

		for (const auto vertex : temp)
			S.push_back(vertex);

		S.push_back(getVertexBySelfIndex(currentIndex));	// salesman has to come back to start vertex
	}

	pair<size_t, size_t> randomVertexes()	// get a pair of random vertexes
	{
		size_t randVertexIndex1 = 1 + (rand() % (S.size() - 2));
		size_t randVertexIndex2 = 1 + (rand() % (S.size() - 2));
		return pair<size_t, size_t>(randVertexIndex1, randVertexIndex2);
	}

	void putToVertex(Vertex* v, Vertex* v2, WEIGHT_TYPE weight) { // put edge between vertexes
		this->self[v].push_back(v2);
		this->self[v2].push_back(v);
		this->edges.push_back(new Edge(v->getIndex(), v2->getIndex(), weight));
		this->edges.push_back(new Edge(v2->getIndex(), v->getIndex(), weight));
	}

	void putToVertex(size_t indexV1, size_t indexV2, WEIGHT_TYPE weight) {
		Vertex* v1 = this->getVertexBySelfIndex(indexV1);
		Vertex* v2 = this->getVertexBySelfIndex(indexV2);
		this->putToVertex(v1, v2, weight);
	}

	void changeS() {
		pair<size_t, size_t> indexes = randomVertexes();	// swap S elements
		swap(S[indexes.first], S[indexes.second]);
	}

	void decreaseTemperature() {
		temperature *= 0.1;
	}

	void setProbability(int deltaE) {
		probability = exp(-deltaE / temperature);
	}

	bool makeTransition() {	// is there a probability to make transition
		if (probability > 1 || probability < 0)
			return false;

		double value = doubleRand(0, 1);
		return (value <= probability);
	}

	void simulatedAnnealing() {
		setVertexes();
		setStartS();
		vector<Vertex*> newPath;
		vector<Vertex*> exPath;
		WEIGHT_TYPE weight = 0;
		double deltaE = 0.0;

		exPath = S;	// save S

		weight = getPathWeight(exPath);	// get exPath weight

		while (temperature > endTemperature) {	// while temperature is quite high

			changeS();

			newPath = S;	// save new S

			if (weight <= getPathWeight(newPath))	// if exPath weight is less than new path weight...
			{
				setProbability(deltaE);	// ...give new path a chance by calculating the transition probability
				if (makeTransition())	// if transition will be made...
				{
					exPath = newPath;	// ...set new path as exPath and set it's weight
					weight = getPathWeight(newPath);
				}
			}
			else
			{
				exPath = newPath;	
				weight = getPathWeight(newPath);
			}

			decreaseTemperature();

			newPath.clear();
		}

		cout << "\n------------------\n";
		for (const auto& v : exPath)
			cout << v->getIndex() << ' ';
		cout << "Path weight: " << weight;
		cout << "\n------------------\n";
	}

	void printGraph()
	{
		for (GRAPH_TYPE::iterator it = this->self.begin(); it != this->self.end(); it++) {	// iterate through graph, get edges adjacent to the vertexes
			vector<Vertex*> vertexes = (*it).second;		
			for (size_t vertexIndex = 0; vertexIndex < vertexes.size(); vertexIndex++) {
				Edge* e = this->getEdgeByVertexes((*it).first, vertexes[vertexIndex]);
			}
		}
	}

	void updateUsedGraph()
	{
		probability = PROBABILITY;
		temperature = TEMPERATURE;
		endTemperature = ENDTEMPERATURE;
		S.clear();
	}

	WEIGHT_TYPE getPathWeight(const vector<Vertex*>& path) {	// weight of calculated path
		WEIGHT_TYPE pathWeight = 0;
		for (int i = 1; i < path.size(); i++)
			pathWeight += getEdgeByVertexes(path[i], path[i - 1])->getWeight();
		return pathWeight;
	}
};

int main()
{
	srand(time(0));
	Graph g;
	ifstream f = ifstream("D:/input.txt");
	if (f) {
		while (!f.eof()) {
			size_t index, leadingIndex;
			WEIGHT_TYPE weight;
			f >> index >> leadingIndex >> weight;
			g.putToVertex(index, leadingIndex, weight);
		}
	}
	else {
		exit(66);
	}

	const int COUNT = 100;
	for (int i = 0; i < COUNT; i++)
	{
		g.simulatedAnnealing();
		g.updateUsedGraph();	// update graph after iteration
	}

	return 0;
}
