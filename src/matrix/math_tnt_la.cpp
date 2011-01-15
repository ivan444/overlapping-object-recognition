#include "math_tnt_la.hpp"
#include "math_tnt.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>


#include <iostream>
#include <algorithm>


typedef boost::numeric::ublas::matrix<double> Matrix;
typedef boost::numeric::ublas::vector<double> Vector;
typedef boost::numeric::ublas::matrix_row<Matrix> MatrixRow;
typedef boost::numeric::ublas::matrix_row<const Matrix> MatrixRowConst;
typedef boost::numeric::ublas::matrix_column<Matrix> MatrixCol;
typedef boost::numeric::ublas::matrix_range<Matrix> MatrixRange;
typedef boost::numeric::ublas::vector_range<Vector> VectorRange;
typedef boost::numeric::ublas::range Range;

//////////////////////////////////
// local helpers

namespace{
  void print(char const* intro, const Matrix& A){
    std::cerr <<intro <<"\n";
    for (unsigned i=0; i<A.size1(); ++i){
      for (unsigned j=0; j<A.size2(); ++j){
        std::cerr <<A(i,j) <<" ";
      }
      std::cerr <<"\n";
    }
  }
  
  double Pi=4*atan(1.0);
}

//////////////////////////////////
// public operations

double math::tnt::lu(
  const Matrix& A,  
  Matrix& L, Matrix& U)
{
  return luTNT(A,L,U);
}
double math::tnt::det(
  const Matrix& A)
{
  Matrix L, U;
  return lu(A,L,U);
}

double math::tnt::svd(
  const Matrix& A,  
  Matrix& U, Matrix& S, Matrix& V)
{
  size_t m=A.size1();
  size_t n=A.size2();
  if (m<n){
    Matrix A0(n,n), U0(n,n), S0(n,n);
    A0.clear();
    MatrixRange(A0, Range (0, m), Range (0, n))=A;
    svdTNT(A0,U0,S0,V);
    U=MatrixRange(U0, Range (0, m), Range (0, m));
    S=MatrixRange(S0, Range (0, m), Range (0, n));
  } else{
    svdTNT(A,U,S,V);
  }
  size_t k=std::min(m,n);
  return S(0,0)/S(k-1,k-1);
}


int math::tnt::invert(
  const Matrix& A,  
  Matrix& Ai)
{
  Matrix U,S,V;
  math::tnt::svd(A, U,S,V);

  //find Sx in order to build generalized inverse
  //(aka pseudoinverse, or Moore-Penrose inverse)  
  size_t m=A.size1();
  size_t n=A.size2();
  size_t k=std::min(m,n);
  size_t rv=k;
  Matrix Sx(trans(S));
  double s_max=S(0,0);
  //double s_min=S(k-1,k-1);
  //double cn=s_max/s_min;
  for (size_t i=0; i<k; ++i){
    double s=S(i,i);
    if (s/s_max<1e-8){
      Sx(i,i)=0;
      --rv;
    } else{
      Sx(i,i)=1/s;
    }
  }
    
  Ai.resize(n,m);
  Matrix SxUt(prod(Sx, trans(U)));
  Ai=prod(V, SxUt);
  return (int)rv;
}


double math::tnt::bestRotationMatrix(
  const Matrix& A,  
  Matrix& B)
{
  size_t m=A.size1();
  assert(m==A.size2());

  Matrix U,S,V;
  math::tnt::svd(A, U,S,V);

  B.resize(m,m);
  B=prod(U, trans(V));
  
  //rotoinversion check 
  double d=det(B);
  if (d<0){
    for (unsigned i=0; i<m; ++i){
      B(i,m-1)=-B(i,m-1);
    }
  }  
  
  return norm_frobenius(B-A);
}


double math::tnt::solveHomogeneous(
  const Matrix& A, Vector& x)
{
  Matrix U,S,V;
  math::tnt::svd(A, U,S,V);

  size_t n=A.size2();
  x = MatrixCol(V, n-1);

  double rn=norm_2(prod(A,x));

#ifndef NDEBUG
  double rnth=(A.size1()<n)?0:S(n-1,n-1);
  double error=rn-rnth;
  assert(error<1e-10);
#endif

  return rn;
}

double math::tnt::solveInhomogeneous(
  const Matrix& A, 
  const Vector& b,
  Vector& x)
{
  size_t m=A.size1();
  size_t n=A.size2();
  Matrix Ab(m,n+1);

  MatrixRange(Ab, Range (0, m), Range (0, n))=A;
  MatrixCol(Ab, n)=b;

  Vector x1(n+1);
  double rn=solveHomogeneous(Ab, x1);

  x1=x1/x1[n];
  x=VectorRange(x1, Range (0, n));

  return rn;
}

/*


////////////////////////////////
// test driver

// g++ math_la*.cpp

// mathematica input
// M = {{.5, .3, -.7}, {.4, .9, -.3}, {.4, .3, .8}}
// b = {1, 2, 3}
// Solve[M.{x1, x2, x3} == b, {x1, x2, x3}]
namespace{
  Matrix M33(){
    double buf[]={.5,.3,-.7, .4,.9,-.3, .4,.3,.8};
    Matrix A(3,3);
    math::tnt::copy(buf,A);
    return A;
  }
  Matrix M32(){
    double buf[]={2,3, 1,1, 4,3};
    Matrix A(3,2);
    math::tnt::copy(buf,A);
    return A;
  }
  Matrix M23(){
    double buf[]={2,3,-8, 1,1,-3};
    Matrix A(2,3);
    math::tnt::copy(buf,A);
    return A;
  }
  Vector b3(){
    Vector b(3);
    double buf[]={1,2,3};
    math::tnt::copy(buf,b);
    return b;
  }
  

  void testSvd(const Matrix& A){
    print("A=", A);
    Matrix U,S,V;
    double cn=math::tnt::svd(A, U,S,V);
    print("U=", U);
    print("S=", S);
    print("V=", V);
    std::cerr <<"cn=" <<cn <<"\n";
  }

  void testSvd(){
    std::cout <<"\nFinding SVD 3x3\n";
    testSvd(M33());
    std::cout <<"\nFinding SVD 2x3\n";
    testSvd(M23());
    std::cout <<"\nFinding SVD 3x2\n";
    testSvd(M32());
  }  
  
  void testInvert(const Matrix& A){
    print("A=", A);
    Matrix Ai;
    int r=math::tnt::invert(A, Ai);
    print("Ai=", Ai);
    std::cerr <<"rank=" <<r <<"\n";
  }
  void testInvert(){
    std::cout <<"\nFinding inverse 3x3\n";
    testInvert(M33());
    std::cout <<"\nFinding inverse 2x3\n";
    testInvert(M23());
  }  

  void testBrm(const Matrix& A){
    print("A=", A);
    Matrix B;
    double mag=math::tnt::bestRotationMatrix(A, B);
    print("B=", B);
    std::cerr <<"norm=" <<mag <<"\n";
  }
  void testBrm(){
    std::cout <<"\nFinding best rotation matrix\n";
    testBrm(M33());
  }  

  void testSolveHom(const Matrix& A){
    std::cout <<"A=" <<A <<"\n";
    Vector x(A.size2());
    double rv=math::tnt::solveHomogeneous(A,x);
    std::cout <<"Solution of Ax=0, x=" <<x <<"\n";
    std::cout <<"Normalized form: xn=" <<x/x(x.size()-1) <<"\n";
    std::cout <<"residual norm=" <<rv <<std::endl;
  }
  void testSolveHom(){
    std::cout <<"\nSolving a homogeneous system \n";
    testSolveHom(M23());
  }
  void testSolveInhom(){
    std::cout <<"\nSolving an inhomogeneous system \n";
    std::cout <<"A=" <<M33() <<"\n";
    std::cout <<"b=" <<b3() <<"\n";
    Vector x(3);
    double rv=math::tnt::solveInhomogeneous(M33(),b3(),x);
    std::cout <<"Solution of Ax=b, x=" <<x <<"\n";
    std::cout <<"residual norm=" <<rv <<std::endl;
  }
}

int main(){
  testSvd();
  testInvert();
  testBrm();

  testSolveHom();
  testSolveInhom();

  return 0;
}
*/
