#pragma once

// Variant 12:
//   computations: A1, A2, B1, B2, C1, C2, D
//   slow: A1, B1 (7 s), the rest are quick (1 s)
//   dependencies: B1(A1), B2(A2), C1(A1), C2(A2), D(B1, B2)
void work();