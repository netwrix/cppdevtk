/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppdevtk/util/damerau_levenshtein_distance.hpp>

#include <QtCore/QString>
#include <QtCore/QChar>

#include <vector>
#include <algorithm>


namespace cppdevtk {
namespace util {



// NOTE: original implementation by Anders Sewerin Johansen (commments kept)
// http://www.merriampark.com/ldcpp.htm
// http://www.talkativeman.com/levenshtein-distance-algorithm-string-comparison/
// Modified for QString/QChar, some formatting

CPPDEVTK_UTIL_API int DamerauLevenshteinDistance(const QString& source, const QString& target) {
	// Step 1
	const int n = source.length();
	const int m = target.length();
	if (n == 0) {
		return m;
	}
	if (m == 0) {
		return n;
	}
	
	// Good form to declare a TYPEDEF
	typedef ::std::vector< ::std::vector<int> > Tmatrix;
	
	Tmatrix matrix(n + 1);
	
	// Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
	// allow for allocation on declaration of 2.nd dimension of vec of vec
	for (int i = 0; i <= n; i++) {
		matrix[i].resize(m + 1);
	}
	
	// Step 2
	for (int i = 0; i <= n; i++) {
		matrix[i][0]=i;
	}
	for (int j = 0; j <= m; j++) {
		matrix[0][j]=j;
	}
	
	// Step 3
	for (int i = 1; i <= n; i++) {
		const QChar s_i = source[i - 1];
		
		// Step 4
		for (int j = 1; j <= m; j++) {
			const QChar t_j = target[j - 1];
			
			// Step 5
			int cost;
			if (s_i == t_j) {
				cost = 0;
			}
			else {
				cost = 1;
			}
			
			// Step 6
			const int above = matrix[i - 1][j];
			const int left = matrix[i][j - 1];
			const int diag = matrix[i - 1][j - 1];
			int cell = ::std::min((above + 1), ::std::min((left + 1), (diag + cost)));
			
			// Step 6A: Cover transposition, in addition to deletion,
			// insertion and substitution. This step is taken from:
			// Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
			// Enhanced Dynamic Programming ASM Algorithm"
			// (http://www.acm.org/~hlb/publications/asm/asm.html)
			if ((i > 2) && (j > 2)) {
				int trans = matrix[i - 2][j - 2] + 1;
				if (source[i - 2] != t_j) trans++;
				if (s_i != target[j - 2]) trans++;
				if (cell > trans) cell = trans;
			}
			matrix[i][j] = cell;
		}
	}
	
	// Step 7
	return matrix[n][m];
}


}	// namespace util
}	// namespace cppdevtk
