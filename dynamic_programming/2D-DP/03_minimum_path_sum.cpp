/*
====================================================================
PROBLEM: Minimum Path Sum
PATTERN: Grid DP (Cost Minimization)

--------------------------------------------------------------------
PROBLEM SUMMARY
--------------------------------------------------------------------
- You are at the top-left corner of an n x m grid.
- Each cell contains a non-negative cost.
- You can move only RIGHT or DOWN.
- Find the minimum cost path to reach bottom-right cell.

--------------------------------------------------------------------
KEY OBSERVATION
--------------------------------------------------------------------
To reach cell (i, j), you can only come from:
- top  -> (i-1, j)
- left -> (i, j-1)

So:
f(i, j) = grid[i][j] + min(f(i-1, j), f(i, j-1))

--------------------------------------------------------------------
CRITICAL PITFALL (INTERVIEW BUG)
--------------------------------------------------------------------
- INT_MAX is used to represent invalid paths.
- NEVER do: grid[i][j] + INT_MAX
  → This causes integer overflow (undefined behavior).
- Always:
  1) Take min() FIRST
  2) Check if result == INT_MAX
  3) Only then add grid[i][j]

--------------------------------------------------------------------
BASE CASES
--------------------------------------------------------------------
- (0, 0) → grid[0][0]
- Out of bounds → INT_MAX

--------------------------------------------------------------------
SOLUTIONS IMPLEMENTED
--------------------------------------------------------------------
1) Pure Recursion        → Exponential (for intuition)
2) Memoization (DP)     → O(n*m)
3) Tabulation (DP)      → O(n*m)
4) Space Optimization   → O(m)

--------------------------------------------------------------------
INTERVIEW TAKEAWAY
--------------------------------------------------------------------
If cost is involved:
- Use min/max transitions
- Be paranoid about INT_MAX
====================================================================
*/

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------------------------------------------
   APPROACH 1: Pure Recursion (INTUITION ONLY)
   TC: Exponential
   SC: O(n + m)
---------------------------------------------------------------- */
class MinPathSumRecursion {
private:
    int f(int i, int j, vector<vector<int>>& grid) {
        if (i == 0 && j == 0) return grid[0][0];
        if (i < 0 || j < 0) return INT_MAX;

        int top  = f(i - 1, j, grid);
        int left = f(i, j - 1, grid);

        int best = min(top, left);
        if (best == INT_MAX) return INT_MAX;

        return grid[i][j] + best;
    }

public:
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        return f(n - 1, m - 1, grid);
    }
};

/* ---------------------------------------------------------------
   APPROACH 2: Memoization (Top-Down DP)
   TC: O(n * m)
   SC: O(n * m) + recursion stack
---------------------------------------------------------------- */
class MinPathSumMemoization {
private:
    int f(int i, int j, vector<vector<int>>& grid,
          vector<vector<int>>& dp) {

        if (i == 0 && j == 0) return grid[0][0];
        if (i < 0 || j < 0) return INT_MAX;

        if (dp[i][j] != -1)
            return dp[i][j];

        int top  = f(i - 1, j, grid, dp);
        int left = f(i, j - 1, grid, dp);

        // CRITICAL: guard INT_MAX before addition
        int best = min(top, left);
        if (best == INT_MAX)
            return dp[i][j] = INT_MAX;

        return dp[i][j] = grid[i][j] + best;
    }

public:
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<vector<int>> dp(n, vector<int>(m, -1));
        return f(n - 1, m - 1, grid, dp);
    }
};

/* ---------------------------------------------------------------
   APPROACH 3: Tabulation (Bottom-Up DP)
   TC: O(n * m)
   SC: O(n * m)
   NOTE: No INT_MAX risk due to proper initialization
---------------------------------------------------------------- */
class MinPathSumTabulation {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<vector<int>> dp(n, vector<int>(m, 0));

        dp[0][0] = grid[0][0];

        // First column
        for (int i = 1; i < n; i++)
            dp[i][0] = grid[i][0] + dp[i - 1][0];

        // First row
        for (int j = 1; j < m; j++)
            dp[0][j] = grid[0][j] + dp[0][j - 1];

        // Remaining cells
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                dp[i][j] = grid[i][j] +
                           min(dp[i - 1][j], dp[i][j - 1]);
            }
        }

        return dp[n - 1][m - 1];
    }
};

/* ---------------------------------------------------------------
   APPROACH 4: Space Optimized
   TC: O(n * m)
   SC: O(m)
---------------------------------------------------------------- */
class MinPathSumSpaceOptimized {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<int> prev(m, 0), curr(m, 0);

        prev[0] = grid[0][0];
        for (int j = 1; j < m; j++)
            prev[j] = grid[0][j] + prev[j - 1];

        for (int i = 1; i < n; i++) {
            curr[0] = grid[i][0] + prev[0];
            for (int j = 1; j < m; j++) {
                curr[j] = grid[i][j] +
                          min(prev[j], curr[j - 1]);
            }
            prev = curr;
        }

        return prev[m - 1];
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    MinPathSumSpaceOptimized sol;
    cout << sol.minPathSum(grid) << endl;

    return 0;
}