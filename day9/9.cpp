#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

void split(const std::string &row, std::vector<int> &array)
{
  std::string symbol;
  for (char c : row)
  {
    array.push_back(c - '0');
  }
}

long find_risk(const std::vector<std::vector<int>> &input)
{
  long risk = 0;

  for (size_t i = 0; i < input.size(); i++)
  {
    for (size_t j = 0; j < input[i].size(); j++)
    {
      if (i > 0 && input[i - 1][j] <= input[i][j])
        continue;
      if (j > 0 && input[i][j - 1] <= input[i][j])
        continue;
      if (i < input.size() - 1 && input[i + 1][j] <= input[i][j])
        continue;
      if (j < input[i].size() - 1 && input[i][j + 1] <= input[i][j])
        continue;

      risk += (input[i][j] + 1);
    }
  }
  return risk;
}

int dfs(int i, int j, const std::vector<std::vector<int>> &input, std::vector<std::vector<bool>> &marked, size_t m, size_t n)
{
  if (i < 0 || j < 0 || i >= m || j >= n || marked[i][j] == true)
    return 0;
  marked[i][j] = true;
  return 1 + dfs(i - 1, j, input, marked, m, n) + dfs(i, j - 1, input, marked, m, n) + dfs(i + 1, j, input, marked, m, n) + dfs(i, j + 1, input, marked, m, n);
}

void init_array(std::vector<std::vector<bool>> &marked, size_t m, size_t n, const std::vector<std::vector<int>> &input)
{
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      if (input[i][j] == 9)
        marked[i][j] = true;
    }
  }
}

int find_basin(const std::vector<std::vector<int>> &input)
{
  size_t m = input.size();
  size_t n = input[0].size();
  std::vector<std::vector<bool>> marked(m, std::vector<bool>(n, false));
  init_array(marked, m, n, input);
  std::vector<int> res;
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      if (marked[i][j])
        continue;
      res.push_back(dfs(i, j, input, marked, m, n));
    }
  }

  std::sort(res.begin(), res.end());
  return res[res.size() - 1] * res[res.size() - 2] * res[res.size() - 3];
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::vector<int>> input;
  while (!in.eof())
  {
    std::vector<int> v;
    std::getline(in, row);
    split(row, v);
    input.push_back(v);
  }
  std::cout << find_risk(input) << "-" << find_basin(input) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}