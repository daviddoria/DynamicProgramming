#ifndef DynamicProgramming_h
#define DynamicProgramming_h

// STL
#include <vector>
#include <stdexcept>

#include "Index.h"

// Eigen
#include <Eigen/Dense>

template <typename TLabel>
class DynamicProgramming
{

public:

  typedef Eigen::Matrix<Index, Eigen::Dynamic, Eigen::Dynamic> IndexMatrixType;

  void SetLabelSet(const std::vector<TLabel>& labels);
  void SetNumberOfNodes(const unsigned int numberOfNodes);
  std::vector<unsigned int> Optimize();

protected:
  virtual float UnaryEnergy(const TLabel& label, const unsigned int node) = 0;
  virtual float BinaryEnergy(const TLabel& labelA, const unsigned int nodeA,
                             const TLabel& labelB, const unsigned int nodeB) = 0;

  std::vector<TLabel> LabelSet;
  unsigned int NumberOfNodes;



  void ComputeGrids();

  std::vector<Index> TracePath();

  Eigen::MatrixXf CostGrid;
  IndexMatrixType PredecessorGrid;
};

#include "DynamicProgramming.hpp"

#endif
