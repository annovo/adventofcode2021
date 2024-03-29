#include <iostream>
#include <utility>

const std::pair<int, int> x = {150, 171};
const std::pair<int, int> y = {-129, -70};

bool in_range(int i_x, int i_y)
{
  int x_c = 0;
  int y_c = 0;
  while (y_c >= y.first && x_c <= x.second)
  {
    if (y_c >= y.first && y_c <= y.second && x_c >= x.first && x_c <= x.second)
      return true;

    if (i_x > 0)
    {
      x_c += i_x;
      i_x--;
    }

    y_c += i_y;
    i_y--;
  }
  return false;
}

void find_range()
{
  // part 1
  int y_max = y.first * (-1) - 1;
  std::cout << (y_max + 1) * y_max / 2 << "\n";

  // part 2
  int pairs = 0;

  for (int i = 0; i <= x.second; i++)
  {
    for (int j = y.first; j <= y_max; j++)
    {
      if (in_range(i, j))
        pairs++;
    }
  }
  std::cout << pairs << "\n";
}

int main(int argc, char *argv[])
{
  find_range();
}