#include <string>
#include <fstream>
#include <iostream>
#include <queue>

//part 1
int findIncrease(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  int sum = 0;
  int prev = INT16_MAX;
  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;

    int curr = std::stoi(row);
    if (curr > prev)
      sum++;
    prev = curr;
  }
  return sum;
}

//part 2
int findIncreaseSum(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::queue<int> q;
  int sum = 0;
  int step = 4;
  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;

    int curr = std::stoi(row);
    q.push(curr);

    if (q.size() == step)
    {
      int prev = q.front();
      q.pop();
      if (prev < curr)
        sum++;
    }
  }
  return sum;
}

int main(int argc, char *argv[])
{
  std::cout << findIncrease(argv[1]) << "\n"
            << findIncreaseSum(argv[1]);
}