/*
====================================================================
PROBLEM: Ninja Training
PATTERN: DP with Last Choice Constraint
(aka Paint House / Activity Selection DP)

--------------------------------------------------------------------
PROBLEM SUMMARY
--------------------------------------------------------------------
- There are n days.
- Each day has 3 tasks with given points.
- You must choose exactly one task per day.
- You CANNOT choose the same task on consecutive days.
- Goal: maximize total points.

--------------------------------------------------------------------
KEY OBSERVATION
--------------------------------------------------------------------
The choice on the current day depends ONLY on:
→ which task was chosen on the previous day.

Hence, the DP state must include the "last task".

--------------------------------------------------------------------
DP STATE DEFINITION
--------------------------------------------------------------------
dp[day][last] = maximum points till 'day'
                if the task done on the previous day was 'last'

last values:
- 0,1,2 → actual tasks
- 3     → no restriction (used for day 0)

--------------------------------------------------------------------
TRANSITION
--------------------------------------------------------------------
For each (day, last):
- Try all tasks {0,1,2}
- Skip task == last
- Take max over:
    points[day][task] + dp[day-1][task]

--------------------------------------------------------------------
BASE CASE
--------------------------------------------------------------------
day = 0:
dp[0][last] = max(points[0][task]) for all task != last

--------------------------------------------------------------------
SOLUTIONS IMPLEMENTED (FOR REVISION)
--------------------------------------------------------------------
1) Pure Recursion
   - TC: O(2^n)
   - SC: O(n)

2) Memoization (Top-Down DP)
   - TC: O(n)
   - SC: O(n)

3) Tabulation (Bottom-Up DP)
   - TC: O(n)
   - SC: O(n)

4) Space Optimization
   - TC: O(n)
   - SC: O(1)

--------------------------------------------------------------------
INTERVIEW TAKEAWAY
--------------------------------------------------------------------
If today's choice is restricted by yesterday's choice,
the DP state MUST include the "last choice".

This pattern appears in:
- Ninja Training
- Paint House
- Stock with cooldown
- Task scheduling with restrictions
====================================================================
*/

#include <bits/stdc++.h>
using namespace std;

/* ---------------------------------------------------------------
   APPROACH 1: Pure Recursion (Exponential)
---------------------------------------------------------------- */
class NinjaTrainingRecursion {
private:
    int f(int day, int last, vector<vector<int>>& points) {
        if(day == 0) {
            int res = 0;
            for(int task = 0; task < 3; task++) {
                if(task != last) {
                    res = max(res, points[0][task]);
                }
            }
            return res;
        }

        int res = 0;
        for(int task = 0; task < 3; task++) {
            if(task != last) {
                res = max(res, points[day][task] + f(day - 1, task, points));
            }
        }
        return res;
    }

public:
    int ninjaTraining(vector<vector<int>>& points) {
        int n = points.size();
        return f(n - 1, 3, points);
    }
};

/* ---------------------------------------------------------------
   APPROACH 2: Memoization (Top-Down DP)
---------------------------------------------------------------- */
class NinjaTrainingMemo {
private:
    int f(int day, int last, vector<vector<int>>& points, vector<vector<int>>& dp) {
        if(day == 0) {
            int res = 0;
            for(int task = 0; task < 3; task++) {
                if(task != last) {
                    res = max(res, points[0][task]);
                }
            }
            return res;
        }

        if(dp[day][last] != -1) return dp[day][last];

        int res = 0;
        for(int task = 0; task < 3; task++) {
            if(task != last) {
                int point = points[day][task] + f(day - 1, task, points, dp);
                res = max(res, point);
            }
        }

        return dp[day][last] = res;
    }

public:
    int ninjaTraining(vector<vector<int>>& points) {
        int n = points.size();
        vector<vector<int>> dp(n, vector<int>(4, -1));
        return f(n - 1, 3, points, dp);
    }
};

/* ---------------------------------------------------------------
   APPROACH 3: Tabulation (Bottom-Up DP)
---------------------------------------------------------------- */
class NinjaTrainingTabulation {
public:
    int ninjaTraining(vector<vector<int>>& points) {
        int n = points.size();
        vector<vector<int>> dp(n, vector<int>(4, 0));

        dp[0][0] = max(points[0][1], points[0][2]);
        dp[0][1] = max(points[0][0], points[0][2]);
        dp[0][2] = max(points[0][0], points[0][1]);
        dp[0][3] = max({points[0][0], points[0][1], points[0][2]});

        for(int day = 1; day < n; day++) {
            for(int last = 0; last < 4; last++) {
                dp[day][last] = 0;
                for(int task = 0; task < 3; task++) {
                    if(task != last) {
                        int point = points[day][task] + dp[day - 1][task];
                        dp[day][last] = max(dp[day][last], point);
                    }
                }
            }
        }

        return dp[n - 1][3];
    }
};

/* ---------------------------------------------------------------
   APPROACH 4: Space Optimization (O(1))
---------------------------------------------------------------- */
class NinjaTrainingSpaceOptimized {
public:
    int ninjaTraining(vector<vector<int>>& points) {
        int n = points.size();
        vector<int> prev(4, 0), curr(4, 0);

        prev[0] = max(points[0][1], points[0][2]);
        prev[1] = max(points[0][0], points[0][2]);
        prev[2] = max(points[0][0], points[0][1]);
        prev[3] = max({points[0][0], points[0][1], points[0][2]});

        for(int day = 1; day < n; day++) {
            for(int last = 0; last < 4; last++) {
                curr[last] = 0;
                for(int task = 0; task < 3; task++) {
                    if(task != last) {
                        int point = points[day][task] + prev[task];
                        curr[last] = max(curr[last], point);
                    }
                }
            }
            prev = curr;
        }

        return prev[3];
    }
};

int main() {
    int n;
    cin >> n;

    vector<vector<int>> points(n, vector<int>(3));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < 3; j++) {
            cin >> points[i][j];
        }
    }

    // vector<vector<int>> points = {
    //     {2, 1, 3},
    //     {3, 4, 6},
    //     {10, 1, 6}
    // };

    NinjaTrainingSpaceOptimized sol;
    cout << sol.ninjaTraining(points) << endl;

    return 0;
}
