#ifndef DynamicProgramming_h
#define DynamicProgramming_h

// STL
#include <vector>
#include <stdexcept>

// Eigen
#include <Eigen/Dense>

class DynamicProgramming
{

public:
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

  typedef Eigen::Matrix<Index, Eigen::Dynamic, Eigen::Dynamic> IndexMatrixType;

  void SetLabelSet(const std::vector<float>& labels);
  void SetNumberOfNodes(const unsigned int numberOfNodes);
  std::vector<unsigned int> Optimize();

private:
  std::vector<float> LabelSet;
  unsigned int NumberOfNodes;

  float UnaryEnergy(const float a, const unsigned int position);
  float BinaryEnergy(const float a, const float b);

  void ComputeGrids();

  std::vector<Index> TracePath();

  Eigen::MatrixXf CostGrid;
  IndexMatrixType PredecessorGrid;
};

#endif
