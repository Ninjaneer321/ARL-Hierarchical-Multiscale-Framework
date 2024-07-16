#include <fstream>
#include <iomanip>

//
// compute f(x, y) = x^2 + y^2
//
int main(int, char * [])
{

  //
  // read input file containing x, y
  //
  double x, y;
  std::ifstream inputFile("input");
  inputFile >> x;
  inputFile >> y;
  
  //
  // compute x^2 + y^2
  //
  const double value = x*x + y*y;
  
  //
  // output value
  //
  std::ofstream outputFile("output");
  outputFile << std::setprecision(18);
  outputFile << value;
  
  //
  //
  //
  return 0;

}
