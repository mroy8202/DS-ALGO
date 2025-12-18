/*
====================================================================
PROBLEM: Triangle Minimum Path Sum
PATTERN: Triangle DP / Grid DP (Downward)

--------------------------------------------------------------------
PROBLEM SUMMARY
--------------------------------------------------------------------
- You are given a triangle array of size n.
- From each cell (i, j), you can move to:
  -> directly below     (i+1, j)
  -> diagonally below   (i+1, j+1)
- Find the minimum path sum from top to bottom.

--------------------------------------------------------------------
DP STATE DEFINITION
--------------------------------------------------------------------
DP State:
- f(i, j) = minimum path sum to reach cell (i, j)

Meaning:
- The value stored at dp[i][j] represents the minimum cost required
  to reach triangle[i][j] starting from the top.

--------------------------------------------------------------------
KEY OBSERVATION
--------------------------------------------------------------------
Each cell (i, j) has at most two parents:
- from above        -> (i-1, j)
- from above-left  -> (i-1, j-1)

So the recurrence is:

f(i, j) = triangle[i][j] + min(f(i-1, j), f(i-1, j-1))

--------------------------------------------------------------------
CRITICAL PITFALL (INTERVIEW BUG)
--------------------------------------------------------------------
- Valid column range for row i is: 0 <= j <= i
- Any (i, j) outside this range is INVALID
- NEVER add INT_MAX directly (causes overflow)

--------------------------------------------------------------------
BASE CASES
--------------------------------------------------------------------
- f(0, 0) = triangle[0][0]
- Invalid indices -> INT_MAX

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
"Each row is built from the previous one by taking the minimum of the
 two valid parents, so at any point I only keep one row of DP."
====================================================================
*/

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------------------------------------------
   APPROACH 1: Pure Recursion (INTUITION ONLY)
---------------------------------------------------------------- */
class TriangleMinPathRecursion {
private:
    int f(int i, int j, vector<vector<int>>& triangle) {
        if (i == 0 && j == 0) return triangle[0][0];
        if (j < 0 || j > i) return INT_MAX;

        int up     = f(i - 1, j, triangle);
        int upLeft = f(i - 1, j - 1, triangle);

        int best = min(up, upLeft);
        if (best == INT_MAX) return INT_MAX;

        return triangle[i][j] + best;
    }

public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        int ans = INT_MAX;
        for (int j = 0; j < n; j++)
            ans = min(ans, f(n - 1, j, triangle));
        return ans;
    }
};

/* ---------------------------------------------------------------
   APPROACH 2: Memoization (Top-Down DP)
---------------------------------------------------------------- */
class TriangleMinPathMemoization {
private:
    int f(int i, int j, vector<vector<int>>& triangle,
          vector<vector<int>>& dp) {

        if (i == 0 && j == 0) return triangle[0][0];
        if (j < 0 || j > i) return INT_MAX;

        if (dp[i][j] != -1)
            return dp[i][j];

        int up     = f(i - 1, j, triangle, dp);
        int upLeft = f(i - 1, j - 1, triangle, dp);

        int best = min(up, upLeft);
        if (best == INT_MAX)
            return dp[i][j] = INT_MAX;

        return dp[i][j] = triangle[i][j] + best;
    }

public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<vector<int>> dp(n);
        for (int i = 0; i < n; i++)
            dp[i].resize(i + 1, -1);

        int ans = INT_MAX;
        for (int j = 0; j < n; j++)
            ans = min(ans, f(n - 1, j, triangle, dp));
        return ans;
    }
};

/* ---------------------------------------------------------------
   APPROACH 3: Tabulation (Bottom-Up DP)
---------------------------------------------------------------- */
class TriangleMinPathTabulation {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<vector<int>> dp(n);
        for (int i = 0; i < n; i++)
            dp[i].resize(i + 1);

        dp[0][0] = triangle[0][0];

        for (int i = 1; i < n; i++) {
            dp[i][0] = triangle[i][0] + dp[i - 1][0];
            dp[i][i] = triangle[i][i] + dp[i - 1][i - 1];

            for (int j = 1; j < i; j++) {
                dp[i][j] = triangle[i][j] +
                           min(dp[i - 1][j - 1], dp[i - 1][j]);
            }
        }

        return *min_element(dp[n - 1].begin(), dp[n - 1].end());
    }
};

/* ---------------------------------------------------------------
   APPROACH 4: Space Optimized DP (RECOMMENDED)
---------------------------------------------------------------- */
class TriangleMinPathSpaceOptimized {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<int> prev;
        prev.push_back(triangle[0][0]);

        for (int i = 1; i < n; i++) {
            vector<int> curr(i + 1);
            curr[0] = triangle[i][0] + prev[0];

            for (int j = 1; j < i; j++) {
                curr[j] = triangle[i][j] +
                          min(prev[j - 1], prev[j]);
            }

            curr[i] = triangle[i][i] + prev[i - 1];
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

    vector<vector<int>> triangle(n);
    for (int i = 0; i < n; i++) {
        triangle[i].resize(i + 1);
        for (int j = 0; j <= i; j++) {
            cin >> triangle[i][j];
        }
    }

    TriangleMinPathSpaceOptimized sol;
    cout << sol.minimumTotal(triangle) << endl;

    return 0;
}
