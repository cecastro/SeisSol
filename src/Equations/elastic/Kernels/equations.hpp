/* If we enable C++11 at some time, the following defines may be replaced
 * by the following code:
constexpr unsigned numberOfBasisFunctions(unsigned O) {
  return O * (O + 1) * (O + 2) / 6;
}

constexpr unsigned numberOfAlignedBasisFunctions(unsigned O) {
  return (numberOfBasisFunctions(O) * sizeof(real) + (ALIGNMENT - (numberOfBasisFunctions(O) * sizeof(real)) % ALIGNMENT) % ALIGNMENT) / sizeof(real);
}

constexpr unsigned numberOfAlignedDerBasisFunctions(unsigned O) {
  return (O > 0) ? numberOfAlignedBasisFunctions(O) + numberOfAlignedDerBasisFunctions(O-1) : 0;
}

#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS numberOfAlignedBasisFunctions(CONVERGENCE_ORDER)
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS numberOfAlignedDerBasisFunctions(CONVERGENCE_ORDER)
 
*/
#if 0
! aligned number of basis functions
#endif

#if ALIGNMENT == 16 && defined(DOUBLE_PRECISION)

#if CONVERGENCE_ORDER == 2
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 4
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 6
#elif CONVERGENCE_ORDER == 3
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 10
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 16
#elif CONVERGENCE_ORDER == 4
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 20
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 36
#elif CONVERGENCE_ORDER == 5
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 36
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 72
#elif CONVERGENCE_ORDER == 6
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 56
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 128
#elif CONVERGENCE_ORDER == 7
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 84
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 212
#elif CONVERGENCE_ORDER == 8
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 120
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 332
#endif

#elif ( ALIGNMENT == 32 && defined(DOUBLE_PRECISION) ) || ( ALIGNMENT == 16 && defined(SINGLE_PRECISION) )

#if CONVERGENCE_ORDER == 2
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 4
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 8
#elif CONVERGENCE_ORDER == 3
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 12
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 20
#elif CONVERGENCE_ORDER == 4
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 20
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 40
#elif CONVERGENCE_ORDER == 5
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 36
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 76
#elif CONVERGENCE_ORDER == 6
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 56
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 132
#elif CONVERGENCE_ORDER == 7
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 84
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 216
#elif CONVERGENCE_ORDER == 8
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 120
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 336
#endif

#elif ( ALIGNMENT == 64 && defined(DOUBLE_PRECISION) ) || ( ALIGNMENT == 32 && defined(SINGLE_PRECISION) )

#if CONVERGENCE_ORDER == 2
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 8
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 16
#elif CONVERGENCE_ORDER == 3
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 16
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 32
#elif CONVERGENCE_ORDER == 4
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 24
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 56
#elif CONVERGENCE_ORDER == 5
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 40
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 96
#elif CONVERGENCE_ORDER == 6
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 56
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 152
#elif CONVERGENCE_ORDER == 7
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 88
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 240
#elif CONVERGENCE_ORDER == 8
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 120
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 360
#endif

#elif ALIGNMENT == 64 && defined(SINGLE_PRECISION)

#if CONVERGENCE_ORDER == 2
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 16
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 32
#elif CONVERGENCE_ORDER == 3
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 16
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 48
#elif CONVERGENCE_ORDER == 4
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 32
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 80
#elif CONVERGENCE_ORDER == 5
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 48
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 128
#elif CONVERGENCE_ORDER == 6
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 64
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 192
#elif CONVERGENCE_ORDER == 7
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 96
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 288
#elif CONVERGENCE_ORDER == 8
#define NUMBER_OF_ALIGNED_BASIS_FUNCTIONS 128
#define NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS 416
#endif

#else

#error alignment-precision combination not implemented.

#endif

#define NUMBER_OF_DOFS         (NUMBER_OF_BASIS_FUNCTIONS             * NUMBER_OF_QUANTITIES)
#define NUMBER_OF_ALIGNED_DOFS (NUMBER_OF_ALIGNED_BASIS_FUNCTIONS     * NUMBER_OF_QUANTITIES)
#define NUMBER_OF_ALIGNED_DERS (NUMBER_OF_ALIGNED_DER_BASIS_FUNCTIONS * NUMBER_OF_QUANTITIES)