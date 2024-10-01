#include <iostream>
#include <list>
#include <limits>
#include <queue>
#include <vector>

#define INF std::numeric_limits<uint>::max() 
#define TAB_SIZE 8

using namespace std;

typedef unsigned int Vertex, uint;

enum Color { WHITE, GRAY, BLACK };

const int moves[8][2] = {
  {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
  {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
};

class Node {
private:
  Vertex value;
  Node *previous;
  Color color;
  uint dist;
public:
  Node() : value(-1), previous(nullptr), color(WHITE), dist(INF) {}
  Node(Vertex v) : value(v), previous(nullptr), color(WHITE), dist(INF) {}
  
  void setValue(Vertex v) { value = v; }
  Vertex getValue() const { return value; }
  void setColor(Color c) { color = c; }
  Color getColor() const { return color; }
  void setPrevious(Node *prev) { previous = prev; }
  Node *getPrevious() const { return previous; }
  void setDist(uint dist) { this->dist = dist; }
  uint getDist() const { return dist; }
  
  bool operator==(const Node &other) const { return value == other.value; }
};

class Graph {
private:
  uint num_edges, num_vertices;
  list<Node> *adj;
  vector<Node> nodes;
public:
  Graph() : num_edges(0), num_vertices(TAB_SIZE * TAB_SIZE) {
    nodes.resize(num_vertices + 1);
    for (uint i = 0; i < num_vertices + 1; i++)
      nodes[i] = Node(i);
    adj = new list<Node>[num_vertices + 1];
  }

  ~Graph() { delete[] adj; }
  
  Vertex posToVertex(uint i, uint j) const {
    return i * TAB_SIZE + j;
  }

  void addEdge(uint Ui, uint Uj, uint Vi, uint Vj) {
    Vertex u = posToVertex(Ui, Uj);
    Vertex v = posToVertex(Vi, Vj);
    adj[u].push_back(nodes[v]);
    adj[v].push_back(nodes[u]);
    num_edges++;
  }

  list<Node>& getAdj(uint i, uint j) {
    Vertex vertex = posToVertex(i, j);
    return adj[vertex];
  }

  Node& getNode(uint i, uint j) {
    Vertex v = posToVertex(i, j);
    return nodes[v];
  }

  bool insideBoard(int i, int j) {
    return i >= 0 && i < TAB_SIZE && j >= 0 && j < TAB_SIZE;
  }

  void createHorseMovesGraph(uint horse_i, uint horse_j) {
    queue<pair<uint, uint>> Q;
    vector<vector<bool>> visited(TAB_SIZE, vector<bool>(TAB_SIZE, false));
    
    Q.push({horse_i, horse_j});
    visited[horse_i][horse_j] = true;
    
    while (!Q.empty()) {
      pair<uint, uint> pos = Q.front();
      Q.pop();
      
      for (auto move : moves) {
        int new_i = pos.first + move[0];
        int new_j = pos.second + move[1];
        
        if (insideBoard(new_i, new_j) && !visited[new_i][new_j]) {
          addEdge(pos.first, pos.second, new_i, new_j);
          Q.push({new_i, new_j});
          visited[new_i][new_j] = true;
        }
      }
    }
  }
};

pair<int, int> posToCoordinate(const string &pos) {
  uint i = pos[1] - '1';
  uint j = pos[0] - 'a';
  return {i, j};
}

uint BFS(Graph &g, Node &root, pair<int, int> &kingPos) {
  queue<Node*> Q;
  for (int i = 0; i < TAB_SIZE; ++i) {
    for (int j = 0; j < TAB_SIZE; ++j) {
      Node &node = g.getNode(i, j);
      node.setColor(WHITE);
      node.setDist(INF);
      node.setPrevious(nullptr);
    }
  }
  
  root.setColor(GRAY);
  root.setDist(0);
  Q.push(&root);
  
  while (!Q.empty()) {
    Node *node_u = Q.front();
    Q.pop();
    
    Vertex u = node_u->getValue();
    int x = u / TAB_SIZE;
    int y = u % TAB_SIZE;

    if (make_pair(x, y) == kingPos) {
      return node_u->getDist();
    }

    for (auto &node_v : g.getAdj(x, y)) {
      if (node_v.getColor() == WHITE) {
        node_v.setColor(GRAY);
        node_v.setDist(node_u->getDist() + 1);
        node_v.setPrevious(node_u);
        Q.push(&node_v);
      }
    }
    node_u->setColor(BLACK);
  }
  return INF;
}

int main() {
  int tests;
  cin >> tests;
  vector<string> positions(5);
  vector<vector<string>> rounds(tests, positions);

  for (int t = 0; t < tests; ++t)
    for (int i = 0; i < 5; ++i)
      cin >> rounds[t][i];

  for (int t = 0; t < tests; ++t) {
    pair<int, int> horses[4], king;
    for (int i = 0; i < 4; ++i)
      horses[i] = posToCoordinate(rounds[t][i]);
    king = posToCoordinate(rounds[t][4]);

    for (auto &horse : horses) {
      Graph g;
      g.createHorseMovesGraph(horse.first, horse.second);
      uint u_i = horse.first;
      uint u_j = horse.second;
      Node &horseNode = g.getNode(u_i, u_j);
      uint dist = BFS(g, horseNode, king);
      cout << dist - 1 << " ";
    }
  }

  return 0;
}
