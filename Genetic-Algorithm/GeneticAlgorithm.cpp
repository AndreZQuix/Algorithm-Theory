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
#define INDIVIDUAL_TYPE WEIGHT_TYPE, vector<Vertex*>	// individual: a path and it's weight
#define PAIR_TYPE pair<pair<INDIVIDUAL_TYPE>, pair<INDIVIDUAL_TYPE>>	// a pair of individuals

const int MAX_POPULATION_SIZE = 10;
const int MUTATION_PERCENT = 75; // mutation probability in percents
const int NUMBER_OF_AGES = 10;

#define random(a, b) (a + (rand() % b))

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
	bool usedAsParent = false;

public:
	size_t getIndex() const {
		return selfIndex;
	}
	Vertex(size_t index) {
		this->selfIndex = index;
	}
	void setUsedAsParent() {
		usedAsParent = true;
	}
	bool getUsedAsParent() const {
		return usedAsParent;
	}
	void setUnusedAsParent() {
		usedAsParent = false;
	}
};

class Graph {

	GRAPH_TYPE self;
	vector<Edge*> edges;	// graph edges
	vector<Vertex*> vertexes = vector<Vertex*>();

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

	multimap<INDIVIDUAL_TYPE>* createPopulation()	// get map of random individuals (population)
	{
		multimap<INDIVIDUAL_TYPE>* population = new multimap<INDIVIDUAL_TYPE>();
		auto rng = std::default_random_engine{};
		for (int i = 0; i < MAX_POPULATION_SIZE; i++)
		{
			vector<Vertex*> temp;	// create temp vector
			temp = vertexes;
			shuffle(begin(temp), end(temp), rng);	// shuffle it
			for (multimap<INDIVIDUAL_TYPE>::iterator it = population->begin(); it != population->end(); ++it)
			{
				if (temp == (*it).second)	// if there is already such an individual in the population...
				{
					shuffle(begin(temp), end(temp), rng);	// ...create new individual by shuffling
					it = population->begin();
				}
			}
			population->insert(pair<INDIVIDUAL_TYPE>(getPathWeight(temp), temp));	// add individual to the population
		}
		return population;
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

	pair<INDIVIDUAL_TYPE> getRandomIndividual(const multimap<INDIVIDUAL_TYPE>* population)
	{
		auto firstIt = population->begin();
		advance(firstIt, random(0, population->size()));
		return *firstIt;
	}

	PAIR_TYPE getParents(const multimap<INDIVIDUAL_TYPE>* population) {	// get two random parents to create a child
		auto firstInd = getRandomIndividual(population);
		auto secondInd = getRandomIndividual(population);
		while (firstInd == secondInd)		// if two random individuals is the same individual...
			secondInd = getRandomIndividual(population);	// ...shuffle
		
		auto parents = make_pair(firstInd, secondInd);	// parents creating (pair of individuals)
		return parents;
	}

	PAIR_TYPE* crossingOver(const PAIR_TYPE& parents) {
		int breakingPoint = random(0, (vertexes.size() - 1));	// generate breaking point

		PAIR_TYPE* children = new PAIR_TYPE();	// pair of children
		
		for(int counter = 0; counter < 2; counter++)	// crossing over goes two times for two childs
		{
			auto tempFirstParent = parents.first;
			auto tempSecondParent = parents.second;
			if (counter == 1)
				swap(tempFirstParent, tempSecondParent);

			pair<INDIVIDUAL_TYPE> child = pair<INDIVIDUAL_TYPE>();	// child
			for (int i = 0; i < breakingPoint; i++)	// copy elements from first parent to the breaking point
			{
				child.second.push_back(tempFirstParent.second[i]);
				tempFirstParent.second[i]->setUsedAsParent();	// vertexes with same values (both parents) is marked as used
				auto anotherParent = *find(tempSecondParent.second.begin(), tempSecondParent.second.end(), tempFirstParent.second[i]);
				anotherParent->setUsedAsParent();
			}

			for (int i = breakingPoint; i < vertexes.size(); i++)	// after the break point, the genes of the second parent are taken
			{
				if (tempSecondParent.second[i]->getUsedAsParent() == false)	// if gnome of the parent wasn't taken...
					child.second.push_back(tempSecondParent.second[i]);	// ..push it to the child

				auto anotherParent = *find(tempFirstParent.second.begin(), tempFirstParent.second.end(), tempSecondParent.second[i]);
				anotherParent->setUsedAsParent();	// another parent gnome is marked as used

				// pay attention: in this case there is no need to mark as used second parent gnome,
				// because they wouldn't be used in current iteration
			}

			while (child.second.size() != vertexes.size())	// if there is empty places in child...
			{
				auto parent = *find_if(tempFirstParent.second.begin(), tempFirstParent.second.end(), [](const Vertex* v) { return !v->getUsedAsParent(); });
				child.second.push_back(parent);	// ...look for UNused gnomes in the first (!) parent
				parent->setUsedAsParent();
			}

			child.first = getPathWeight(child.second);	// calculate child fitness (path weight)

			for (auto& v : tempFirstParent.second)	// unmark parents flags: all genes are marked as unused
				v->setUnusedAsParent();

			for (auto& v : tempSecondParent.second)
				v->setUnusedAsParent();

			counter == 0 ? children->first = child : children->second = child;

			// there is 2 iterations: first child is created, which takes the initial genes up to the breaking point of the first parent,
			// then on second iteration second child takes the initial genes up to the breaking point of the second parent
		}

		return children;	// return a pair of children
	}

	void swapGnome(pair<INDIVIDUAL_TYPE>* child) {
		size_t firstRandomGnome = random(0, (vertexes.size() - 1));	
		size_t secondRandomGnome = random(0, (vertexes.size() - 1));
		while (firstRandomGnome == secondRandomGnome)
			secondRandomGnome = random(0, (vertexes.size() - 1));
		swap(child->second[firstRandomGnome], child->second[secondRandomGnome]);
		child->first = getPathWeight(child->second);
	}

	void mutation(PAIR_TYPE* children) {
		int mutationChance = random(0, 100);
		if (mutationChance < MUTATION_PERCENT)	// if mutation occurs...
		{
			swapGnome(&children->first);	// ...swap two random genes of child
			swapGnome(&children->second);
		}
	}

	void geneticAlgorithm() {
		setVertexes();
		multimap<INDIVIDUAL_TYPE> *population = createPopulation();	// genereate random population

		for (int counter = 0; counter < NUMBER_OF_AGES; counter++)
		{
			PAIR_TYPE parents = getParents(population);
			PAIR_TYPE* children = crossingOver(parents);
			mutation(children);	// children mutation

			population->insert(children->first);	// mutated children enter the population
			population->insert(children->second);

			size_t countIndividualsToDelete = population->size() - MAX_POPULATION_SIZE;	// calculate the amount of individuals to delete
			population->erase(prev(population->end(), countIndividualsToDelete), population->end()); // delete last extra individuals from population

			cout << "\nThe most adapted individual in the population:\n";	// there is sorting by path weight: from largest to smallest
			for (const auto& ind : population->begin()->second)			// so, the most adapted individuals will be located in the beginning of the population 
				cout << ind->getIndex() << ' ';
			cout << " Weight: " << population->begin()->first;

			delete children;
		}

		delete population;
	}

	void printGraph()
	{
		for (GRAPH_TYPE::iterator it = this->self.begin(); it != this->self.end(); it++) {
			vector<Vertex*> vertexes = (*it).second;	// // iterate through graph, get edges adjacent to the vertexes
			for (size_t vertexIndex = 0; vertexIndex < vertexes.size(); vertexIndex++) {	// iterate through these edges
				Edge* e = this->getEdgeByVertexes((*it).first, vertexes[vertexIndex]);
			}
		}
	}

	WEIGHT_TYPE getPathWeight(const vector<Vertex*>& path) {
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

	map<vector<Vertex*>, WEIGHT_TYPE> paths;

	const int COUNT = 10;
	for (int i = 0; i < COUNT; i++)
	{
		cout << "Iteration #" << i << " started!\n";
		g.geneticAlgorithm();
		cout << "\nIteration #" << i << " succeed!\n";
	}

	return 0;
}
