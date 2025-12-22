/*
============================================================================
PROBLEM: Cherry Pickup II
PLATFORM: LeetCode
PATTERN: 3D Dynamic Programming (Two Agents) + Space Optimization
============================================================================

PROBLEM SUMMARY
----------------------------------------------------------------------------
- You are given an n x m grid.
- Two robots start at row 0:
    - Robot 1 starts at column 0
    - Robot 2 starts at column m-1
- Both robots move down one row at a time.
- Each robot can move to:
    - left-down (col - 1)
    - down      (col)
    - right-down(col + 1)
- If both robots land on the same cell, cherries are counted only once.
- Goal: Maximize total cherries collected.

----------------------------------------------------------------------------

DP STATE DEFINITION
----------------------------------------------------------------------------
dp[i][j1][j2] = maximum cherries collectable starting from
                row i, where:
                - robot1 is at column j1
                - robot2 is at column j2

Since dp[i] depends only on dp[i+1], we optimize space to 2 layers.

----------------------------------------------------------------------------

BASE CASE
----------------------------------------------------------------------------
At the last row (i = n - 1):
- If j1 == j2 → grid[n-1][j1]
- Else        → grid[n-1][j1] + grid[n-1][j2]

----------------------------------------------------------------------------

TRANSITION
----------------------------------------------------------------------------
From (i, j1, j2), both robots can move:
dj1 ∈ {-1, 0, +1}
dj2 ∈ {-1, 0, +1}

Try all 9 combinations and take the maximum.

----------------------------------------------------------------------------

FINAL ANSWER
----------------------------------------------------------------------------
Robots start at:
- row = 0
- columns = (0, m-1)

Answer = dp[0][0][m-1]

============================================================================
*/

#include <bits/stdc++.h>
using namespace std;

static const int NEG_INF = -1000000000;

/*===========================================================================
  1. RECURSION (FOR DERIVATION / UNDERSTANDING)

  Time Complexity  : O(9^n)
  Space Complexity : O(n)
=============================================================================*/

class CherryPickupRecursion {
public:
    int solveRec(int i, int j1, int j2, int n, int m, vector<vector<int>>& grid) {
        if(j1 < 0 || j1 >= m || j2 < 0 || j2 >= m)
            return NEG_INF;

        if(i == n - 1) {
            if(j1 == j2) return grid[i][j1];
            return grid[i][j1] + grid[i][j2];
        }

        int best = NEG_INF;

        for(int dj1 = -1; dj1 <= 1; dj1++) {
            for(int dj2 = -1; dj2 <= 1; dj2++) {
                int curr = (j1 == j2)
                           ? grid[i][j1]
                           : grid[i][j1] + grid[i][j2];

                curr += solveRec(i + 1, j1 + dj1, j2 + dj2, n, m, grid);
                best = max(best, curr);
            }
        }

        return best;
    }
};

/*===========================================================================
  2. MEMOIZATION (TOP-DOWN DP)

  Time Complexity  : O(n * m^2)
  Space Complexity : O(n * m^2) + O(n)
=============================================================================*/

class CherryPickupMemoization {
public:
    int solveMemo(int i, int j1, int j2, int n, int m,
                  vector<vector<int>>& grid,
                  vector<vector<vector<int>>>& dp) {

        if(j1 < 0 || j1 >= m || j2 < 0 || j2 >= m)
            return NEG_INF;

        if(i == n - 1) {
            if(j1 == j2) return grid[i][j1];
            return grid[i][j1] + grid[i][j2];
        }

        if(dp[i][j1][j2] != -1)
            return dp[i][j1][j2];

        int best = NEG_INF;

        for(int dj1 = -1; dj1 <= 1; dj1++) {
            for(int dj2 = -1; dj2 <= 1; dj2++) {
                int curr = (j1 == j2)
                           ? grid[i][j1]
                           : grid[i][j1] + grid[i][j2];

                curr += solveMemo(i + 1, j1 + dj1, j2 + dj2, n, m, grid, dp);
                best = max(best, curr);
            }
        }

        return dp[i][j1][j2] = best;
    }
};

/*===========================================================================
  3. TABULATION (BOTTOM-UP DP)

  Time Complexity  : O(n * m^2)
  Space Complexity : O(n * m^2)
=============================================================================*/

class CherryPickupTabulation {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<vector<vector<int>>> dp(
            n, vector<vector<int>>(m, vector<int>(m, NEG_INF))
        );

        for(int j1 = 0; j1 < m; j1++) {
            for(int j2 = 0; j2 < m; j2++) {
                if(j1 == j2)
                    dp[n - 1][j1][j2] = grid[n - 1][j1];
                else
                    dp[n - 1][j1][j2] = grid[n - 1][j1] + grid[n - 1][j2];
            }
        }

        for(int i = n - 2; i >= 0; i--) {
            for(int j1 = 0; j1 < m; j1++) {
                for(int j2 = 0; j2 < m; j2++) {

                    int best = NEG_INF;

                    for(int dj1 = -1; dj1 <= 1; dj1++) {
                        for(int dj2 = -1; dj2 <= 1; dj2++) {

                            int nj1 = j1 + dj1;
                            int nj2 = j2 + dj2;

                            if(nj1 < 0 || nj1 >= m || nj2 < 0 || nj2 >= m)
                                continue;

                            int curr = (j1 == j2)
                                       ? grid[i][j1]
                                       : grid[i][j1] + grid[i][j2];

                            curr += dp[i + 1][nj1][nj2];
                            best = max(best, curr);
                        }
                    }

                    dp[i][j1][j2] = best;
                }
            }
        }

        return dp[0][0][m - 1];
    }
};

/*===========================================================================
  4. SPACE OPTIMIZED TABULATION

  Time Complexity  : O(n * m^2)
  Space Complexity : O(m^2)
=============================================================================*/

class CherryPickupSpaceOptimized {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<vector<int>> prev(m, vector<int>(m, NEG_INF));
        vector<vector<int>> curr(m, vector<int>(m, NEG_INF));

        for(int j1 = 0; j1 < m; j1++) {
            for(int j2 = 0; j2 < m; j2++) {
                if(j1 == j2)
                    prev[j1][j2] = grid[n - 1][j1];
                else
                    prev[j1][j2] = grid[n - 1][j1] + grid[n - 1][j2];
            }
        }

        for(int i = n - 2; i >= 0; i--) {
            for(int j1 = 0; j1 < m; j1++) {
                for(int j2 = 0; j2 < m; j2++) {

                    int best = NEG_INF;

                    for(int dj1 = -1; dj1 <= 1; dj1++) {
                        for(int dj2 = -1; dj2 <= 1; dj2++) {

                            int nj1 = j1 + dj1;
                            int nj2 = j2 + dj2;

                            if(nj1 < 0 || nj1 >= m || nj2 < 0 || nj2 >= m)
                                continue;

                            int currVal = (j1 == j2)
                                          ? grid[i][j1]
                                          : grid[i][j1] + grid[i][j2];

                            currVal += prev[nj1][nj2];
                            best = max(best, currVal);
                        }
                    }

                    curr[j1][j2] = best;
                }
            }
            prev = curr;
        }

        return prev[0][m - 1];
    }
};

/*===========================================================================
  DRIVER FUNCTION
=============================================================================*/

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> grid(n, vector<int> (m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    CherryPickupSpaceOptimized Sol;
    cout << Sol.cherryPickup(grid) << endl;

    return 0;
}