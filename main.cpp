#include <iostream>
#include <list>
#include <limits>
#include <vector>
#include <queue>

#define INF std::numeric_limits<uint>::infinity()

using namespace std;

typedef unsigned int Vertex;
typedef unsigned int uint;

enum Color { WHITE, GRAY, BLACK };

class Node {
private:
  Vertex value;
  Node *previous;
  Color color;
  uint t0, t, dist;
public:
Node() : value(-1), previous(nullptr), color(WHITE), t0(0), t(0) {
  dist = INF;
}
Node(Vertex v) : value(v), previous(nullptr), color(WHITE), t0(0), t(0) {
  dist = INF;
}
  void setValue(Vertex v) { value = v; }
  Vertex getValue() const { return value; }
  void setColor(Color c) { color = c; }
  Color getColor() const { return color; }
  void setPrevious(Node *prev) { previous = prev; }
  Node *getPrevious() const { return previous; }
  void setT(uint t) { this->t = t; }
  uint getT() const { return t; }
  void setT0(uint t0) { this->t0 = t0; }
  uint getT0() const { return t0; }
  void setDist(uint dist) { this->dist = dist; }
  uint getDist() const { return dist; }
  // for remove_edge()
  bool operator==(const Node &other) const { return value == other.value; }
};

template <typename T> class Graph {
private:
  uint num_vertices;
  uint num_edges;
  list<T> *adj;
  vector<T> nodes;
public:
  Graph(uint num_vertices) : num_vertices(num_vertices), num_edges(0) {
    nodes.resize(num_vertices + 1);
    for (uint i = 0; i < num_vertices + 1; i++) {
      nodes[i] = T(i);
    }
    adj = new list<T>[num_vertices + 1];
  }
  ~Graph() { delete[] adj; }
  uint get_num_vertices() const { return num_vertices; }
  uint get_num_edges() const { return num_edges; }
  void add_edge(Vertex u, Vertex v) {
    T Node_u(u);
    T Node_v(v);
    adj[u].push_back(Node_v);
    adj[v].push_back(Node_u);
    num_edges++;
  }
  list<T>& get_adj(Vertex vertex) const {
    return adj[vertex];
  }
  void remove_edge(Vertex u, Vertex v) {
    T Node_u(u);
    T Node_v(v);
    adj[u].remove(Node_v);
    adj[v].remove(Node_u);
    num_edges--;
  }

  void show_graph() const {
    cout << "num_vertices: " << num_vertices << endl;
    cout << "num_edges: " << num_edges << endl;
    for (Vertex v = 0; v < num_vertices; v++) {
      cout << v << ": ";
      list<T> adj_list = get_adj(v);
      for (const auto &vertex : adj_list)
        cout << vertex.getValue() << ", ";
      cout << endl;
    }
  }

  void printVertexDegreeSum() {
    int sum = 0;
    for (Vertex v = 1; v <= num_vertices; v++) {
      int degree = get_adj(v).size();
      sum += degree;
    }
    cout << "Sum of degrees: " << sum << endl;
  }

  bool isComplete() const {
    for (Vertex v = 1; v <= num_vertices; v++) {
      int degree = get_adj(v).size();
      if (degree + 1 != num_vertices)
        return false;
    }
    return true;
  }

  vector<T> getNodes() {
    return nodes;
  }
};

template <typename T> void DFS(Graph<T> &g) {
  uint time = 0;
  for (Vertex u = 0; u < g.get_num_vertices(); u++) {
    for (auto &node_u : g.get_adj(u))
      if (node_u.getColor() == WHITE)
        DFS_visit(g, node_u, time);
  }
}

template <typename T> void DFS_visit(Graph<T> &g, T &node_u, uint &time) {
  node_u.setColor(GRAY);
  ++time;
  node_u.setT0(time);
  cout << "t0: " << node_u.getT0() << "; vertex: " << node_u.getValue() << endl;
  
  for (auto &node_v : g.get_adj(node_u.getValue())) {
    if (node_v.getColor() == WHITE) {
      node_v.setPrevious(&node_u);
      DFS_visit(g, node_v, time);
    }
  }

  node_u.setColor(BLACK);
  ++time;
  node_u.setT(time);
  cout << "t: " << node_u.getT() << "; vertex: " << node_u.getValue() << endl;
}

template <typename T> void BFS(Graph<T> &g, T &root) {
  for (T &node : g.getNodes()) {
    node.setColor(WHITE);
    node.setDist(INF);
    node.setPrevious(nullptr);
  }
  root.setColor(GRAY);
  root.setDist(0);
  queue<T*> Q;
  Q.push(&root);
  
  while (!Q.empty()) {
    T *node_u = Q.front();
    Q.pop();
    
    for (auto &node_v : g.get_adj(node_u->getValue())) {
      cout << node_v.getColor() << endl;
      if (node_v.getColor() == WHITE) {
        node_v.setColor(GRAY);
        node_v.setDist(node_u->getDist() + 1);
        node_v.setPrevious(node_u);
        Q.push(&node_v);
        cout << "dist: " << node_v.getDist() << "; vertex: " << node_v.getValue() << endl;
      }
    }
    node_u->setColor(BLACK);
  }
}

int main() {
  uint n_edges, n_vertices;
  cin >> n_vertices >> n_edges;
  Graph<Node> g(n_vertices);
  Vertex u, v;
  for (uint i = 0; i < n_edges; i++) {
    cin >> u >> v;
    g.add_edge(u, v);
  }
  g.show_graph();
  cout << "<==== DFS ====>" << endl;
  DFS(g);
  
  cout << "<==== BFS ====>" << endl;
  Node root = g.getNodes()[1];
  BFS(g, root);
  return 0;
}