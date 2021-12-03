#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

enum Dir
{
  Horizontal,
  Down,
  Up
};
static std::unordered_map<std::string, Dir> const table = {{"forward", Horizontal}, {"down", Down}, {"up", Up}};

std::vector<std::string> split(const std::string &row)
{
  std::vector<std::string> fields{""};
  size_t i = 0;
  for (char c : row)
  {
    switch (c)
    {
    case ' ':
      fields.push_back("");
      i++;
      break;
    default:
      fields[i].push_back(c);
      break;
    }
  }
  return fields;
}

//part 1
int findDepthAndHor(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;

  int h = 0;
  int depth = 0;
  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;
    std::vector<std::string> fields = split(row);
    int val = std::stoi(fields[1]);
    auto it = table.find(fields[0]);
    Dir d = it->second;

    switch (d)
    {
    case Horizontal:
      h += val;
      break;
    case Up:
      depth -= val;
      break;
    case Down:
      depth += val;
      break;
    default:
      break;
    }
  }
  return h * depth;
}

//part 2
int findDepthAndHorWithAim(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;

  int h = 0;
  int depth = 0;
  int aim = 0;
  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;
    std::vector<std::string> fields = split(row);
    int val = std::stoi(fields[1]);
    auto it = table.find(fields[0]);
    Dir d = it->second;
    switch (d)
    {
    case Horizontal:
      h += val;
      depth += aim * val;
      break;
    case Up:
      aim -= val;
      break;
    case Down:
      aim += val;
      break;
    default:
      break;
    }
  }
  return h * depth;
}

int main(int argc, char *argv[])
{
  std::cout << findDepthAndHor(argv[1]) << "\n"
            << findDepthAndHorWithAim(argv[1]);
}