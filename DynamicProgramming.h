#ifndef DynamicProgramming_h
#define DynamicProgramming_h

// STL
#include <vector>

// Eigen
#include <Eigen/Dense>

class DynamicProgramming
{
public:
  void SetLabelSet(const std::vector<float>& labels);
  void SetNumberOfNodes(const unsigned int numberOfNodes);
  std::vector<unsigned int> Optimize();

private:
  std::vector<float> LabelSet;
  unsigned int NumberOfNodes;

  float UnaryEnergy(const float a, const unsigned int position);
  float BinaryEnergy(const float a, const float b);

  Eigen::MatrixXf ComputeGrid();

  std::vector<Eigen::MatrixXf::Index> GetPossiblePredecessors(const Eigen::MatrixXf::Index& index);
};

#endif
