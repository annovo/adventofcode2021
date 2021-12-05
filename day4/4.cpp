#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <utility>

struct Cell
{
  int val;
  int x;
  int y;
  Cell(const int v = 0, const int n_x = 0, const int n_y = 0)
      : val(v),
        x(n_x),
        y(n_y) {}

  bool operator<(const Cell &a) const
  {
    return val < a.val;
  }

  bool operator==(const Cell &a) const
  {
    return val == a.val;
  }
};

class Bingo
{
public:
  std::vector<int> split(const std::string &row, const char splitter)
  {
    std::vector<int> fields{};
    std::string symbol;
    for (char c : row)
    {
      if (c == splitter && !symbol.empty())
      {
        fields.push_back(std::stoi(symbol));
        symbol = "";
      }
      else if (c != splitter)
        symbol.push_back(c);
    }

    if (!symbol.empty())
      fields.push_back(std::stoi(symbol));

    return fields;
  }

  void parse(const std::string &fileName)
  {
    std::ifstream in(fileName);
    std::string row;
    int cell_x = 0;
    int cell_y = 0;
    size_t size_h = 0;
    size_t size_v = 0;
    std::set<Cell> s;

    //get the inputs
    std::getline(in, row);
    input = split(row, ',');

    //skip first empty line
    std::getline(in, row);

    while (!in.eof())
    {
      std::getline(in, row);
      if (row.empty())
      {
        sets.push_back(s);
        s = {};
        size_v = cell_y;
        cell_y = 0;
        continue;
      }

      const std::vector<int> splitted = split(row, ' ');
      for (int v : splitted)
      {
        Cell c(v, cell_x++, cell_y);
        s.insert(c);
      }
      size_h = cell_x;
      cell_y++;
      cell_x = 0;
    }
    sets.push_back(s);
    size_t set_size = sets.size();
    horizontal = initVectors(set_size, size_v, true);
    vertical = initVectors(set_size, size_h, false);
  }

  //part 1 & 2
  std::pair<int, int> solve()
  {
    std::vector<std::set<Cell>> sets_lcl(sets);
    std::vector<std::vector<int>> horizontal_lcl(horizontal);
    std::vector<std::vector<int>> vertical_lcl(vertical);
    std::set<int> counter;
    std::pair<int, int> ans(0, 0);

    for (size_t i = 0; i < sets.size(); i++)
    {
      counter.insert(i);
    }

    for (int v : input)
    {
      for (size_t i = 0; i < sets_lcl.size(); i++)
      {
        std::set<int>::iterator it_counter = counter.find(i);
        if (it_counter == counter.end())
          continue;
        std::set<Cell>::iterator it_cell = sets_lcl[i].find(v);
        if (it_cell != sets_lcl[i].end())
        {
          const Cell c = *it_cell;
          horizontal_lcl[i][c.y] |= (1 << c.x);
          vertical_lcl[i][c.x] |= (1 << c.y);
          sets_lcl[i].erase(it_cell);
          if (horizontal_lcl[i][c.y] == mask_h || vertical_lcl[i][c.x] == mask_v)
          {
            if (counter.size() == sets.size())
              ans.first = count(sets_lcl[i], v);
            if (counter.size() == 1)
            {
              ans.second = count(sets_lcl[*(it_counter)], v);
              return ans;
            }
            counter.erase(it_counter);
          }
        }
      }
    }
    return ans;
  }

private:
  std::vector<std::vector<int>> initVectors(const size_t x, const size_t y, const bool horizontal)
  {
    std::vector<std::vector<int>> r;
    for (size_t i = 0; i < x; i++)
    {
      std::vector<int> v;
      for (size_t j = 0; j < y; j++)
      {
        if (i == 0)
        {
          if (horizontal)
          {
            mask_h |= 1 << j;
          }
          else
          {
            mask_v |= 1 << j;
          }
        }
        v.push_back(0);
      }
      r.push_back(v);
    }

    return r;
  }
  int count(const std::set<Cell> &s, const int v)
  {
    int r = 0;
    for (auto el : s)
    {
      r += el.val;
    }
    return r * v;
  }
  std::vector<int> input;
  std::vector<std::set<Cell>> sets;
  std::vector<std::vector<int>> horizontal;
  std::vector<std::vector<int>> vertical;
  int mask_h = 0;
  int mask_v = 0;
};

int main(int argc, char *argv[])
{
  Bingo b;
  b.parse(argv[1]);
  std::pair ans = b.solve();
  std::cout << ans.first << "\n"
            << ans.second << "\n";
}