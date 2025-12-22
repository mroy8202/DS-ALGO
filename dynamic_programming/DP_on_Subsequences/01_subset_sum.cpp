/*
============================================================================
PROBLEM: Subset Sum
PATTERN: 0/1 Knapsack (Decision DP)
============================================================================

PROBLEM STATEMENT
----------------------------------------------------------------------------
Given an array of non-negative integers `arr[]` and an integer `sum`,
determine whether there exists a subset whose sum equals `sum`.

Each element can be used at most once.

----------------------------------------------------------------------------

DP STATE
----------------------------------------------------------------------------
f(ind, s) = true if we can form sum `s` using elements from index [0 ... ind]

In tabulation:
dp[ind][s] = same meaning as above

----------------------------------------------------------------------------

BASE CASES
----------------------------------------------------------------------------
1. If s == 0:
   - Always true (empty subset)

2. If ind == 0:
   - dp[0][s] = true only if arr[0] == s

----------------------------------------------------------------------------

TRANSITION
----------------------------------------------------------------------------
At index `ind`, for target sum `s`:

Option 1: NOT TAKE current element
    notTake = dp[ind - 1][s]

Option 2: TAKE current element (only if arr[ind] <= s)
    take = dp[ind - 1][s - arr[ind]]

dp[ind][s] = take OR notTake

----------------------------------------------------------------------------

FINAL ANSWER
----------------------------------------------------------------------------
Return dp[n - 1][sum]

----------------------------------------------------------------------------

SPACE OPTIMIZATION INSIGHT
----------------------------------------------------------------------------
While converting 2D DP → 1D DP:

- dp[s] depends on dp[s] and dp[s - arr[ind]]
- To avoid reusing the same element multiple times,
  iterate `s` from right to left (sum → 0)

Right-to-left iteration ensures dp values come from
the previous iteration (ind - 1).

----------------------------------------------------------------------------

COMPLEXITY
----------------------------------------------------------------------------
Time Complexity:
- Recursion        : O(2^n)
- Memoization      : O(n * sum)
- Tabulation       : O(n * sum)
- Space Optimized  : O(n * sum)

Space Complexity:
- Recursion        : O(n)
- Memoization      : O(n * sum)
- Tabulation       : O(n * sum)
- 2 Arrays         : O(sum)
- 1 Array          : O(sum)

============================================================================
*/


#include <bits/stdc++.h>
using namespace std;

/*
============================================================================
1. BRUTE FORCE RECURSION
============================================================================
f(ind, sum) -> can we form `sum` using elements [0 ... ind]

TIME COMPLEXITY: O(2^n)
SPACE COMPLEXITY: O(n) (recursion stack)
============================================================================
*/
class SubsetSumRecursive {
private:
    bool solve(int ind, int sum, vector<int>& arr) {
        if(sum == 0) return true;
        if(ind == 0) return (arr[0] == sum);

        bool notTake = solve(ind - 1, sum, arr);

        bool take = false;
        if(arr[ind] <= sum)
            take = solve(ind - 1, sum - arr[ind], arr);

        return take || notTake;
    }

public:
    bool isSubsetSum(vector<int>& arr, int sum) {
        if(arr.empty()) return false;
        return solve(arr.size() - 1, sum, arr);
    }
};


/*
============================================================================
2. TOP-DOWN DP (MEMOIZATION)
============================================================================
TIME COMPLEXITY: O(n * sum)
SPACE COMPLEXITY: O(n * sum) + O(n)
============================================================================
*/
class SubsetSumMemoized {
private:
    bool solve(int ind, int sum, vector<int>& arr, vector<vector<int>>& dp) {
        if(sum == 0) return true;
        if(ind == 0) return (arr[0] == sum);

        if(dp[ind][sum] != -1) return dp[ind][sum];

        bool notTake = solve(ind - 1, sum, arr, dp);

        bool take = false;
        if(arr[ind] <= sum)
            take = solve(ind - 1, sum - arr[ind], arr, dp);

        return dp[ind][sum] = take || notTake;
    }

public:
    bool isSubsetSum(vector<int>& arr, int sum) {
        if(arr.empty()) return false;

        int n = arr.size();
        vector<vector<int>> dp(n, vector<int>(sum + 1, -1));

        return solve(n - 1, sum, arr, dp);
    }
};


/*
============================================================================
3. BOTTOM-UP DP (TABULATION)
============================================================================
dp[ind][s] -> can we form sum `s` using elements [0 ... ind]

TIME COMPLEXITY: O(n * sum)
SPACE COMPLEXITY: O(n * sum)
============================================================================
*/
class SubsetSumTabulation {
public:
    bool isSubsetSum(vector<int>& arr, int sum) {
        int n = arr.size();
        if(n == 0) return false;

        vector<vector<bool>> dp(n, vector<bool>(sum + 1, false));

        for(int i = 0; i < n; i++) dp[i][0] = true;
        if(arr[0] <= sum) dp[0][arr[0]] = true;

        for(int ind = 1; ind < n; ind++) {
            for(int s = 1; s <= sum; s++) {
                bool notTake = dp[ind - 1][s];
                bool take = (arr[ind] <= s) ? dp[ind - 1][s - arr[ind]] : false;
                dp[ind][s] = take || notTake;
            }
        }

        return dp[n - 1][sum];
    }
};


/*
============================================================================
4. SPACE OPTIMIZED DP (2 ARRAYS)
============================================================================
TIME COMPLEXITY: O(n * sum)
SPACE COMPLEXITY: O(sum)
============================================================================
*/
class SubsetSumSpaceOptimized2D {
public:
    bool isSubsetSum(vector<int>& arr, int sum) {
        int n = arr.size();
        if(n == 0) return false;

        vector<bool> prev(sum + 1, false);
        prev[0] = true;
        if(arr[0] <= sum) prev[arr[0]] = true;

        for(int ind = 1; ind < n; ind++) {
            vector<bool> curr(sum + 1, false);
            curr[0] = true;

            for(int s = 1; s <= sum; s++) {
                bool notTake = prev[s];
                bool take = (arr[ind] <= s) ? prev[s - arr[ind]] : false;
                curr[s] = take || notTake;
            }
            prev = curr;
        }

        return prev[sum];
    }
};


/*
============================================================================
5. SPACE OPTIMIZED DP (1 ARRAY)
============================================================================
IMPORTANT:
    Iterate sum from right to left to prevent reusing the same element.

TIME COMPLEXITY: O(n * sum)
SPACE COMPLEXITY: O(sum)
============================================================================
*/
class SubsetSumSpaceOptimized1D {
public:
    bool isSubsetSum(vector<int>& arr, int sum) {
        int n = arr.size();
        if(n == 0) return false;

        vector<bool> dp(sum + 1, false);
        dp[0] = true;
        if(arr[0] <= sum) dp[arr[0]] = true;

        for(int ind = 1; ind < n; ind++) {
            for(int s = sum; s >= 1; s--) {
                bool notTake = dp[s];
                bool take = (arr[ind] <= s) ? dp[s - arr[ind]] : false;
                dp[s] = take || notTake;
            }
        }

        return dp[sum];
    }
};


/*
============================================================================
MAIN FUNCTION
============================================================================
*/
int main() {
    vector<int> arr = {1, 2, 3, 4};
    int sum = 6;

    SubsetSumSpaceOptimized1D solver;
    cout << solver.isSubsetSum(arr, sum) << endl;

    return 0;
}