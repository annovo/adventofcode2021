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

void split(const std::string &row, std::vector<int> &a)
{
  for (char c : row)
    a.push_back(c - '0');
}

int calculate_val(const std::vector<std::vector<int>> &input, int m, int n, int x, int y, int sign, bool is_x)
{
  int val = is_x ? ((input[(x + sign) % m][y % n] + (x + sign) / m + y / n)) : ((input[x % m][(y + sign) % n] + (y + sign) / n + x / m));
  return val >= 10 ? val % 10 + 1 : val;
}

void update_dist(std::priority_queue<Vertex, std::vector<Vertex>, LessThanByWeight> &pq,
                 std::vector<std::vector<int>> &paths,
                 const std::vector<std::vector<bool>> &marked,
                 const std::vector<std::vector<int>> &input,
                 const Vertex &v, bool part_one)
{
  std::vector<Vertex> v_arr;
  const int m = input.size();
  const int n = input[0].size();
  if (v.x > 0)
    v_arr.push_back({v.x - 1, v.y, calculate_val(input, m, n, v.x, v.y, -1, true)});
  if (v.y > 0)
    v_arr.push_back({v.x, v.y - 1, calculate_val(input, m, n, v.x, v.y, -1, false)});
  if ((part_one && v.x < m - 1) || (!part_one && v.x < (m * part_two_size - 1)))
    v_arr.push_back({v.x + 1, v.y, calculate_val(input, m, n, v.x, v.y, 1, true)});
  if ((part_one && v.y < n - 1) || (!part_one && v.y < (n * part_two_size - 1)))
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

int find_shortest_path(const std::vector<std::vector<int>> &input, bool part_one)
{
  std::priority_queue<Vertex, std::vector<Vertex>, LessThanByWeight> pq;
  int p = part_one ? 1 : part_two_size;
  std::vector<std::vector<int>> paths(input.size() * p, std::vector<int>(input[0].size() * p, INT16_MAX));
  std::vector<std::vector<bool>> marked(input.size() * p, std::vector<bool>(input[0].size() * p, false));

  paths[0][0] = 0;
  pq.push({0, 0, 0});
  while (!pq.empty())
  {
    auto v = pq.top();
    marked[v.x][v.y] = true;
    pq.pop();
    update_dist(pq, paths, marked, input, v, part_one);
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
  std::cout << find_shortest_path(input, true) << "\n";
  // part 2
  std::cout << find_shortest_path(input, false) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}