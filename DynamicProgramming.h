#ifndef DynamicProgramming_h
#define DynamicProgramming_h

// STL
#include <vector>
#include <stdexcept>

// Custom
#include "Index.h"

// Eigen
#include <Eigen/Dense>

template <typename TLabel, typename TNode>
class DynamicProgramming
{
public:

  /** Define the type of the matrix to use to store the predecessors. */
  typedef Eigen::Matrix<Index, Eigen::Dynamic, Eigen::Dynamic> IndexMatrixType;

  /** Set the label set. */
  void SetLabelSet(const std::vector<TLabel>& labels);

  /** Set the nodes. */
  void SetNodes(const std::vector<TNode>& nodes);

  /** Perform the dynamic programming to compute the best labels to apply to the nodes. */
  std::vector<unsigned int> Optimize();

protected:
  /** Compute the cost of assigning a label to a node. */
  virtual float UnaryEnergy(const TLabel& label, const TNode& node) = 0;

  /** Compute the cost of assigning labels to nodes. */
  virtual float BinaryEnergy(const TLabel& labelA, const TNode& nodeA,
                             const TLabel& labelB, const TNode& nodeB) = 0;

  /** Set the label set. */
  std::vector<TLabel> LabelSet;

  /** Set the nodes. */
  std::vector<TNode> Nodes;

  /** Compute the cost and predecessor grids. */
  void ComputeGrids();

  /** Compute the resulting best labeling by tracing back through the grids. */
  std::vector<Index> TracePath();

  /** The grid of costs of labels vs nodes. */
  Eigen::MatrixXf CostGrid;

  /** The grid of predecessors indicating which label was best in the previous node row. */
  IndexMatrixType PredecessorGrid;
};

#include "DynamicProgramming.hpp"

#endif
