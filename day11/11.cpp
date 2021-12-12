#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

void split(const std::string &row, std::vector<int> &array)
{
  for (char c : row)
    array.push_back(c - '0');
}

void flash(std::vector<std::vector<int>> &input, std::unordered_set<int> &flashed)
{
  int i_size = input.size();
  for (size_t i = 0; i < input.size(); i++)
  {
    for (size_t j = 0; j < input[i].size(); j++)
    {
      input[i][j]++;
    }
  }

  bool iscontinue = true;
  while (iscontinue)
  {
    iscontinue = false;
    for (size_t i = 0; i < input.size(); i++)
    {
      for (size_t j = 0; j < input[i].size(); j++)
      {
        if (input[i][j] <= 9 || flashed.find(i * i_size + j) != flashed.end())
          continue;

        iscontinue = true;
        flashed.insert(i * i_size + j);
        if (i > 0)
          input[i - 1][j]++;
        if (j > 0)
          input[i][j - 1]++;
        if (i > 0 && j > 0)
          input[i - 1][j - 1]++;
        if (i < input.size() - 1)
          input[i + 1][j]++;
        if (j < input[i].size() - 1)
          input[i][j + 1]++;
        if (i < input.size() - 1 && j < input[i].size() - 1)
          input[i + 1][j + 1]++;
        if (i > 0 && j < input[i].size() - 1)
          input[i - 1][j + 1]++;
        if (j > 0 && i < input.size() - 1)
          input[i + 1][j - 1]++;
      }
    }
  }
  for (auto &v : input)
  {
    for (auto &n : v)
    {
      if (n > 9)
        n = 0;
    }
  }
}

void find_flashes(const std::vector<std::vector<int>> &input, int days)
{
  auto array = input;
  long sum = 0;
  int past_days = 0;
  while (true)
  {
    past_days++;
    std::unordered_set<int> flashed;
    flash(array, flashed);

    // part 2
    if (flashed.size() == input.size() * input[0].size())
    {
      std::cout << past_days << "\n";
      break;
    }

    // part 1
    if (past_days == days)
      std::cout << sum << "\n";
    if (past_days < days)
      sum += flashed.size();
  }
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
  find_flashes(input, 100);
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}