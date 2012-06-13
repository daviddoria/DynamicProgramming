#ifndef Index_h
#define Index_h

#include <stdexcept>

struct Index
{
  int row;
  int col;
  Index() : row(-1), col(-1) {}
  Index(const int r, const int c) : row(r), col(c) {}

  int operator[](const int index) const
  {
    if(index == 0)
    {
      return row;
    }
    else if(index == 1)
    {
      return col;
    }
    else
    {
      throw std::runtime_error("Requested invalid index!");
    }
  }

  int& operator[](const int index)
  {
    if(index == 0)
    {
      return row;
    }
    else if(index == 1)
    {
      return col;
    }
    else
    {
      throw std::runtime_error("Requested invalid index!");
    }
  }
};

#endif
