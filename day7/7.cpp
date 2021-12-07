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
    if (isdigit(c))
      symbol.push_back(c);

    if (c == ',' && !symbol.empty())
    {
      array.push_back(std::stoi(symbol));
      symbol = "";
    }
  }
  if (!symbol.empty())
    array.push_back(std::stoi(symbol));
}

long calculate(int a, int b, bool first)
{
  int v = std::abs(a - b);
  //part 1
  if (first)
    return v;

  //part 2
  return (1 + v) * v / 2;
}

long find_fuel(const std::vector<int> &input, bool first)
{
  long max_fuel = INT64_MAX;
  int min_el = INT16_MAX;
  int max_el = INT16_MIN;

  for (int v : input)
  {
    min_el = std::min(min_el, v);
    max_el = std::max(max_el, v);
  }

  for (int i = min_el; i <= max_el; i++)
  {
    long t_sum = 0;
    for (size_t j = 0; j < input.size(); j++)
    {
      if (t_sum > max_fuel)
        break;

      t_sum += calculate(i, input[j], first);
    }
    max_fuel = std::min(t_sum, max_fuel);
  }
  return max_fuel;
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<int> input;
  while (!in.eof())
  {
    std::getline(in, row);
    split(row, input);
  }
  std::cout << find_fuel(input, true) << "-" << find_fuel(input, false) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}