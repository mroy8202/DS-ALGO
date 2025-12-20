/*
====================================================================
PROBLEM: Minimum Falling Path Sum
PATTERN: Grid DP (Downward Multi-Transition)

--------------------------------------------------------------------
PROBLEM SUMMARY
--------------------------------------------------------------------
- Given an n x n matrix mat.
- A falling path starts from any cell in the first row.
- From cell (i, j), you may move to:
  -> (i+1, j)
  -> (i+1, j-1)
  -> (i+1, j+1)
- Find the minimum possible sum of a falling path ending in the last row.

--------------------------------------------------------------------
DP STATE DEFINITION
--------------------------------------------------------------------
f(i, j) = minimum falling path sum to reach cell (i, j)

Meaning:
- f(i, j) stores the minimum cost to reach mat[i][j]
  starting from any cell in the first row.

--------------------------------------------------------------------
RECURRENCE
--------------------------------------------------------------------
f(i, j) = mat[i][j] + min(
              f(i-1, j),
              f(i-1, j-1),
              f(i-1, j+1)
          )

--------------------------------------------------------------------
BASE CASES
--------------------------------------------------------------------
- f(0, j) = mat[0][j]
- Invalid column index -> INT_MAX

--------------------------------------------------------------------
TIME & SPACE COMPLEXITY
--------------------------------------------------------------------
- Recursion:    TC = Exponential, SC = O(n)
- Memoization:  TC = O(n^2),      SC = O(n^2)
- Tabulation:   TC = O(n^2),      SC = O(n^2)
- Space Opt:    TC = O(n^2),      SC = O(n)

--------------------------------------------------------------------
INTERVIEW TAKEAWAY
--------------------------------------------------------------------
Each cell stores the minimum falling path sum ending there, built from
the three possible parents in the row above.
====================================================================
*/

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------------------------------------------
   APPROACH 1: Pure Recursion (INTUITION ONLY)
---------------------------------------------------------------- */
class MinFallingPathRecursion {
private:
    int f(int i, int j, vector<vector<int>>& mat) {
        int n = mat.size();
        if(j < 0 || j >= n) return INT_MAX;
        if(i == 0) return mat[0][j];

        int up = f(i - 1, j, mat);
        int upLeft = f(i - 1, j - 1, mat);
        int upRight = f(i - 1, j + 1, mat);

        int best = min({up, upLeft, upRight});
        if(best == INT_MAX) return INT_MAX;

        return mat[i][j] + best;
    }

public:
    int minFallingPathSum(vector<vector<int>>& mat) {
        int n = mat.size();
        int ans = INT_MAX;

        for(int j = 0; j < n; j++) {
            ans = min(ans, f(n - 1, j, mat));
        }
        return ans;
    }
};

/* ---------------------------------------------------------------
   APPROACH 2: Memoization (Top-Down DP)
---------------------------------------------------------------- */
class MinFallingPathMemoization {
private:
    int f(int i, int j, vector<vector<int>>& mat, vector<vector<int>>& dp) {
        int n = mat.size();
        if(j < 0 || j >= n) return INT_MAX;
        if(i == 0) return mat[0][j];

        if(dp[i][j] != INT_MAX) return dp[i][j];

        int up = f(i - 1, j, mat, dp);
        int upLeft = f(i - 1, j - 1, mat, dp);
        int upRight = f(i - 1, j + 1, mat, dp);

        int best = min({up, upLeft, upRight});
        if(best == INT_MAX) return dp[i][j] = INT_MAX;

        return dp[i][j] = mat[i][j] + best;
    }

public:
    int minFallingPathSum(vector<vector<int>>& mat) {
        int n = mat.size();
        vector<vector<int>> dp(n, vector<int>(n, INT_MAX));

        int ans = INT_MAX;
        for(int j = 0; j < n; j++) {
            ans = min(ans, f(n - 1, j, mat, dp));
        }
        return ans;
    }
};

/* ---------------------------------------------------------------
   APPROACH 3: Tabulation (Bottom-Up DP)
---------------------------------------------------------------- */
class MinFallingPathTabulation {
public:
    int minFallingPathSum(vector<vector<int>>& mat) {
        int n = mat.size();
        vector<vector<int>> dp(n, vector<int>(n));

        dp[0] = mat[0];

        for(int i = 1; i < n; i++) {
            for(int j = 0; j < n; j++) {
                int best = dp[i - 1][j];
                if(j > 0) best = min(best, dp[i - 1][j - 1]);
                if(j < n - 1) best = min(best, dp[i - 1][j + 1]);

                dp[i][j] = mat[i][j] + best;
            }
        }

        return *min_element(dp[n - 1].begin(), dp[n - 1].end());
    }
};

/* ---------------------------------------------------------------
   APPROACH 4: Space Optimized DP (RECOMMENDED)
---------------------------------------------------------------- */
class MinFallingPathSpaceOptimized {
public:
    int minFallingPathSum(vector<vector<int>>& mat) {
        int n = mat.size();
        vector<int> prev = mat[0];
        vector<int> curr(n);

        for(int i = 1; i < n; i++) {
            for(int j = 0; j < n; j++) {
                int best = prev[j];
                if(j > 0) best = min(best, prev[j - 1]);
                if(j < n - 1) best = min(best, prev[j + 1]);

                curr[j] = mat[i][j] + best;
            }
            prev = curr;
        }

        return *min_element(prev.begin(), prev.end());
    }
};

/* ---------------------------------------------------------------
   DRIVER CODE
---------------------------------------------------------------- */
int main() {
    int n;
    cin >> n;

    vector<vector<int>> mat(n, vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }

    MinFallingPathSpaceOptimized sol;
    cout << sol.minFallingPathSum(mat) << endl;

    return 0;
}