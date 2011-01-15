#ifndef math_tnt_la_hpp
#define math_tnt_la_hpp

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace math{
namespace tnt{

//returns determinant
double lu(
  const boost::numeric::ublas::matrix<double>& A,  
  boost::numeric::ublas::matrix<double>& L,
  boost::numeric::ublas::matrix<double>& U);
double det(
  const boost::numeric::ublas::matrix<double>& A);

//returns condition number
double svd(
  const boost::numeric::ublas::matrix<double>& A,  
  boost::numeric::ublas::matrix<double>& U,
  boost::numeric::ublas::matrix<double>& S,
  boost::numeric::ublas::matrix<double>& V);

//returns rank
int invert(
  const boost::numeric::ublas::matrix<double>& A,  
  boost::numeric::ublas::matrix<double>& Ai);

//returns norm(Q-R)
double bestRotationMatrix(
  const boost::numeric::ublas::matrix<double>& Q,  
  boost::numeric::ublas::matrix<double>& R);

//all solvers return residual norm
double solveHomogeneous(  //Ax=0
  const boost::numeric::ublas::matrix<double>& A,  
  boost::numeric::ublas::vector<double>& x);
double solveInhomogeneous(//Ax+b=0
  const boost::numeric::ublas::matrix<double>& A,  
  const boost::numeric::ublas::vector<double>& b,  
  boost::numeric::ublas::vector<double>& x);



} //end namespace la
} //end namespace math

#endif

