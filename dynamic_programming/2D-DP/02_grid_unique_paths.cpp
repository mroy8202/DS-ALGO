/*
====================================================================
PROBLEM: Unique Paths
PATTERN: Grid DP (Paths Counting)

--------------------------------------------------------------------
PROBLEM SUMMARY
--------------------------------------------------------------------
- You are at the top-left corner of an n x m grid.
- You can move only RIGHT or DOWN.
- Count the number of unique paths to reach bottom-right cell.

--------------------------------------------------------------------
KEY OBSERVATION
--------------------------------------------------------------------
To reach cell (i, j), you can only come from:
- top  -> (i-1, j)
- left -> (i, j-1)

So the number of ways to reach (i, j) is:
ways(i, j) = ways(i-1, j) + ways(i, j-1)

--------------------------------------------------------------------
RECURSIVE FUNCTION DEFINITION
--------------------------------------------------------------------
f(i, j) = number of unique paths to reach cell (i, j)

--------------------------------------------------------------------
BASE CASES
--------------------------------------------------------------------
- If i == 0 and j == 0 → 1 path (starting cell)
- If i < 0 or j < 0 → 0 paths (out of grid)

--------------------------------------------------------------------
DP TRANSITION
--------------------------------------------------------------------
dp[i][j] = dp[i-1][j] + dp[i][j-1]

--------------------------------------------------------------------
SOLUTIONS IMPLEMENTED
--------------------------------------------------------------------
1) Pure Recursion        → Exponential
2) Memoization (DP)     → O(n*m)
3) Tabulation (DP)      → O(n*m)
4) Space Optimization   → O(m)

--------------------------------------------------------------------
INTERVIEW TAKEAWAY
--------------------------------------------------------------------
If movement is restricted to directions (right / down),
think GRID DP and build from top-left to bottom-right.
====================================================================
*/

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------------------------------------------
   APPROACH 1: Pure Recursion (for intuition)
   TC: O(2 ^ n+m) Exponential
   SC: O(n + m) recursion stack
---------------------------------------------------------------- */
class GridUniquePathsRecursion {
private:
    int f(int i, int j) {
        if (i == 0 && j == 0) return 1;
        if (i < 0 || j < 0) return 0;

        int top = f(i - 1, j);
        int left = f(i, j - 1);

        return top + left;
    }

public:
    int uniquePaths(int n, int m) {
        return f(n - 1, m - 1);
    }
};

/* ---------------------------------------------------------------
   APPROACH 2: Memoization (Top-Down DP)
   TC: O(n * m)
   SC: O(n * m) + recursion stack
---------------------------------------------------------------- */
class GridUniquePathsMemoization {
private:
    int f(int i, int j, vector<vector<int>>& dp) {
        if (i == 0 && j == 0) return 1;
        if (i < 0 || j < 0) return 0;

        if (dp[i][j] != -1)
            return dp[i][j];

        int top = f(i - 1, j, dp);
        int left = f(i, j - 1, dp);

        return dp[i][j] = top + left;
    }

public:
    int uniquePaths(int n, int m) {
        vector<vector<int>> dp(n, vector<int>(m, -1));
        return f(n - 1, m - 1, dp);
    }
};

/* ---------------------------------------------------------------
   APPROACH 3: Tabulation (Bottom-Up DP)
   TC: O(n * m)
   SC: O(n * m)
---------------------------------------------------------------- */
class GridUniquePathsTabulation {
public:
    int uniquePaths(int n, int m) {
        vector<vector<int>> dp(n, vector<int>(m, 0));

        // Base cases: first row and first column
        for (int i = 0; i < n; i++) dp[i][0] = 1;
        for (int j = 0; j < m; j++) dp[0][j] = 1;

        // Fill the DP table
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        return dp[n - 1][m - 1];
    }
};

/* ---------------------------------------------------------------
   APPROACH 4: Space Optimization
   TC: O(n * m)
   SC: O(m)
---------------------------------------------------------------- */
class GridUniquePathsSpaceOptimized {
public:
    int uniquePaths(int n, int m) {
        // dp[j] represents number of ways to reach current row, column j
        vector<int> prev(m, 1), curr(m, 0);

        for (int i = 1; i < n; i++) {
            curr[0] = 1;  // first column always has 1 path
            for (int j = 1; j < m; j++) {
                curr[j] = prev[j] + curr[j - 1];
            }
            prev = curr;
        }

        return prev[m - 1];
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    GridUniquePathsSpaceOptimized sol;
    cout << sol.uniquePaths(n, m) << endl;

    return 0;
}