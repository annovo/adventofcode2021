#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

const int part_two_size = 5;
struct Vertex
{
  int x;
  int y;
  int val;
};

struct LessThanByWeight
{
  bool operator()(const Vertex &lhs, const Vertex &rhs) const
  {
    return lhs.val > rhs.val;
  }
};

using VertexQueue = std::priority_queue<Vertex, std::vector<Vertex>, LessThanByWeight>;

void split(const std::string &row, std::vector<int> &a)
{
  for (char c : row)
    a.push_back(c - '0');
}

int calculate_val(const std::vector<std::vector<int>> &input, int m, int n, int x, int y, int sign, bool is_x)
{
  int val = is_x ? ((input[(x + sign) % m][y % n] + (x + sign) / m + y / n)) : ((input[x % m][(y + sign) % n] + (y + sign) / n + x / m));
  return val % 9 == 0 ? val : val % 9;
}

void update_dist(VertexQueue &pq,
                 std::vector<std::vector<int>> &paths,
                 const std::vector<std::vector<bool>> &marked,
                 const std::vector<std::vector<int>> &input,
                 const Vertex &v, int size)
{
  std::vector<Vertex> v_arr;
  const int m = input.size();
  const int n = input[0].size();
  if (v.x > 0)
    v_arr.push_back({v.x - 1, v.y, calculate_val(input, m, n, v.x, v.y, -1, true)});
  if (v.y > 0)
    v_arr.push_back({v.x, v.y - 1, calculate_val(input, m, n, v.x, v.y, -1, false)});
  if (v.x < m * size - 1)
    v_arr.push_back({v.x + 1, v.y, calculate_val(input, m, n, v.x, v.y, 1, true)});
  if (v.y < n * size - 1)
    v_arr.push_back({v.x, v.y + 1, calculate_val(input, m, n, v.x, v.y, 1, false)});

  for (const auto &vertex : v_arr)
  {
    if (marked[vertex.x][vertex.y] == true)
      continue;

    int dist = paths[v.x][v.y] + vertex.val;
    if (paths[vertex.x][vertex.y] > dist)
    {
      paths[vertex.x][vertex.y] = dist;
      pq.push({vertex.x, vertex.y, dist});
    }
  }
}

int find_shortest_path(const std::vector<std::vector<int>> &input, int size)
{
  VertexQueue pq;
  std::vector<std::vector<int>> paths(input.size() * size, std::vector<int>(input[0].size() * size, INT16_MAX));
  std::vector<std::vector<bool>> marked(input.size() * size, std::vector<bool>(input[0].size() * size, false));

  paths[0][0] = 0;
  pq.push({0, 0, 0});
  while (!pq.empty())
  {
    auto v = pq.top();
    pq.pop();
    if (marked[v.x][v.y])
      continue;

    marked[v.x][v.y] = true;
    update_dist(pq, paths, marked, input, v, size);
  }
  return paths[paths.size() - 1][paths[0].size() - 1];
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::vector<int>> input;
  while (!in.eof())
  {
    std::getline(in, row);
    std::vector<int> v;
    split(row, v);
    input.push_back(v);
  }

  // part 1
  std::cout << find_shortest_path(input, 1) << "\n";
  // part 2
  std::cout << find_shortest_path(input, part_two_size) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}