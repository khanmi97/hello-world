# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util
from util import Stack
from util import Queue
from util import PriorityQueue

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"

    if problem.isGoalState(problem.getStartState()): # initial condition where the goal state is found
        return []

    stk = Stack() # keep track of the nodes

    stk.push((problem.getStartState(), [])) # push start state in the stack
    visitedList = []  # list of visited nodes

    flag = True #boolean value for iteration of the loop
    while stk.isEmpty() is not flag:
        if stk.isEmpty() is not flag:
            temp, pathsList = stk.pop() # list of all paths for each state

            #visitedList.add(temp)
            visitedList.append(temp)  # append to visited list

            if problem.isGoalState(temp) is True: # Goal state found; return the paths
                return pathsList
            else:
                nodeList = problem.getSuccessors(temp)  # find successor of 'temp' state
                for element in nodeList:
                    if element[0] not in visitedList:
                        dfsPath = pathsList + [element[1]] # computation of another path
                        stk.push((element[0], dfsPath)) # add states to the stack
        else:
            return []


    util.raiseNotDefined()

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"

    if problem.isGoalState(problem.getStartState()): # initial condition where the goal state is found
        return []

    queue = Queue()
    queue.push((problem.getStartState(), [])) # insert start state in the queue

    visitedList = [] # list of visited nodes

    while queue.isEmpty() is False:
        if queue.isEmpty() is False:
            temp, pathsList = queue.pop() # list of all paths for each state

            if temp not in visitedList: # temp does not exist in visitedList
                visitedList.append(temp) # append to visited list

                if problem.isGoalState(temp) is True: # Goal state found; return the paths
                    return pathsList
                else:
                    sList = problem.getSuccessors(temp) # find successor of 'temp' state
                    for i, j, k in sList:
                        bfsPath = pathsList + [j] # computation of another path
                        queue.push((i, bfsPath)) # add states to queue

        else:
            return[]

    util.raiseNotDefined()

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"


    if problem.isGoalState(problem.getStartState()): # initial condition where start state is goal state
        return []

    visitedList = [] # list of visited nodes

    priQueue = PriorityQueue() # intialize priority queue
    priQueue.push((problem.getStartState(), [], 0), 0) # insert start state in the queue in min priority

    while priQueue.isEmpty() is False:
        if priQueue.isEmpty() is False:
            temp1, pathsList, temp2 = priQueue.pop() # list of all paths for each state
                                                    # temp1 and temp2 are new node and the cost to get to the node respectively

            if temp1 not in visitedList: # temp1 does not exist in visitedList
                visitedList.append(temp1) # append to visited list
                if problem.isGoalState(temp1): # Goal state found; return the paths
                    return pathsList
                else:
                    sList = problem.getSuccessors(temp1) # find successor of 'temp1' state
                    for i, j, k in sList:
                        ucs1 = pathsList + [j] # computation of new node to start node
                        ucs2 = temp2 + k
                        priQueue.push((i, ucs1, ucs2), ucs2)

        else:
            return []


    util.raiseNotDefined()



def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"

    if problem.isGoalState(problem.getStartState()): # initial condition where start state os goal state
        return []

    visitedList = []

    aStarQueue = PriorityQueue()
    aStarQueue.push((problem.getStartState(), [], 0), 0)

    while aStarQueue.isEmpty() is False:
        if aStarQueue.isEmpty() is False:
            node1, hList, node2 = aStarQueue.pop() # list of all paths for each state
                                                    # node1 and node2 are new node and the cost to get to the node respectively

            if node1 not in visitedList:
                visitedList.append(node1)
                if problem.isGoalState(node1):
                    return hList
                else:
                    sList = problem.getSuccessors(node1)
                    for i, j, k in sList:
                        aStar1 = hList + [j]
                        aStar2 = node2 + k
                        aStarQueue.push((i, aStar1, aStar2), aStar2 + heuristic(i, problem)) # sum of heuristic value and cost of path from start to n

            else:
                "The node is already appended to visited nodes"
        else:
            return []

    util.raiseNotDefined()


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
