# multiAgents.py
# --------------
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
from typing import Any, Tuple

from util import manhattanDistance
from game import Directions
import random, util

from game import Agent
from util import manhattanDistance

class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"

        result = 0 # store final value of calculation
        foodList = newFood.asList()
        score = successorGameState.getScore() # will be added to the final result
        ghostPosition = successorGameState.getGhostPositions()
        place = -2
        place2 = 0
        distance1 = 1
        distance2 = 1
        for item in ghostPosition:
            manDist = manhattanDistance(newPos, item)
            if manDist <= 1:
                distance1 += 1
            distance2 = distance2 + manDist

        for element in foodList: # how far is the food pellet
            location = manhattanDistance(newPos, element)
            if place == -2 or location <= place:
                place = location


        result = (1/place) - (1/distance2) - distance1

        return result + score


def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "*** YOUR CODE HERE ***"

        resultList = []

        start = -1e10

        # recursive function for implementing minimaz

        def minimax(gameState, agent, level):

            negInf = -1e10 # start for pacman
            posInf = 1e10 # start for ghosts

            if level == self.depth or not gameState.getLegalActions(agent): # reached deepest level
                return self.evaluationFunction(gameState) # base case
            else:
                # if the agent is a ghost, then minimize
                if agent > 0:
                    ghostAgent = agent + 1 # go to next agent
                    if ghostAgent == gameState.getNumAgents():
                        ghostAgent = 0
                        level = level + 1

                    posInf = min(minimax(gameState.generateSuccessor(agent, item), ghostAgent, level) for item in gameState.getLegalActions(agent))
                    return posInf
                else: # if the agent is pacman, then minimize
                    negInf = max(minimax(gameState.generateSuccessor(agent, item), 1, level) for item in gameState.getLegalActions(0))
                    return negInf

        for item in gameState.getLegalActions(0):
            value = minimax(gameState.generateSuccessor(0, item), 1, 0)
            if start < value or start == -1e10:
                start = value
                result = item


        return result

        util.raiseNotDefined()

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        # With the help of following methods, alpha-beta pruning will be calculated
        def pruning(gameState, alpha, beta, agent, level):
            if agent == 0:
                return max_value(gameState, alpha, beta, level)
            else:
                return min_value(gameState, alpha, beta, agent, level)

        result = 0
        alpha = -1e10
        beta = 1e10
        stateValue = -1e10
        for element in gameState.getLegalActions(0):  # pacman in the game tree from root node
            value = pruning(gameState.generateSuccessor(0, element), alpha, beta, 1, 0)
            if value > stateValue:
                stateValue = value
                result = element
            if stateValue > beta:
                return stateValue
            alpha = max(alpha, stateValue)

        # alpha-beta implementation
        def max_value(gameState, alpha, beta, level): # for pacman, agent = 0
            max = -1e10
            if level == self.depth or not gameState.getLegalActions(0):
                return self.evaluationFunction(gameState)

            for item in gameState.getLegalActions(0):
                max = max(pruning(gameState.generateSuccessor(0, item), alpha, beta, 1, level), max)
                if max > beta:
                    return max
                else:
                    alpha = max(alpha, max)
            return max

        def min_value(gameState, alpha, beta, agent, level): # for ghosts, agent > 0
            min = 1e10
            if level == self.depth or not gameState.getLegalActions(agent):
                return self.evaluationFunction(gameState)
            ghost = agent + 1
            if ghost == gameState.getNumAgents():
                ghost = 0
                level = level + 1

            for item in gameState.getLegalActions(agent):
                min = min(pruning(gameState.generateSuccessor(agent, item), alpha, beta, ghost, level), min)
                if min < alpha:
                    return min
                beta = min(beta, min)
            return min

        return result

        util.raiseNotDefined()

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        util.raiseNotDefined()

def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()

# Abbreviation
better = betterEvaluationFunction
