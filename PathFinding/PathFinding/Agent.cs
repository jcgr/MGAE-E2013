using System.Collections.Generic;

namespace PathFinding
{
    class Agent
    {
        /// <summary>
        /// The x-position of the agent.
        /// </summary>
        public int X { get; set; }
        /// <summary>
        /// The y-position of the agent.
        /// </summary>
        public int Y { get; set; }

        /// <summary>
        /// The speed of the agent.
        /// </summary>
        public int Speed { get; set; }

        /// <summary>
        /// The path the agent is taking.
        /// </summary>
        public List<Node> Path { get; set; }

        /// <summary>
        /// Initializes the Agent with default values.
        /// </summary>
        public Agent()
            : this (0, 0, 1)
        {
        }

        /// <summary>
        /// Initializes the agent.
        /// </summary>
        /// <param name="x">The x-coordinate of the agent.</param>
        /// <param name="y">The y-coordiante of the agent.</param>
        /// <param name="speed">The speed of the agent.</param>
        public Agent(int x, int y, int speed)
        {
            X = x;
            Y = y;
            Speed = speed;
            Path = new List<Node>();
        }

        /// <summary>
        /// Chase the target using the aStar algorithm.
        /// </summary>
        /// <param name="target">The target to chase.</param>
        /// <param name="map">The map the agent is traveling on.</param>
        public void Chase(Agent target, Map map)
        {
            Path = AStar.FindPathTo(map[X, Y], map[target.X, target.Y], map);
        }

        /// <summary>
        /// Simple way to avoid the target.
        /// </summary>
        /// <param name="target">The target to avoid.</param>
        /// <param name="map">The map that the agent is traveling on.</param>
        public void Avoid(Agent target, Map map)
        {
            var iterations = 0;
            int newX = X, newY = Y;

            var targetNode = map[target.X, target.Y];
            var currentDist = map[X, Y].DistanceTo(targetNode);

            // Only 10 steps are needed.
            while (iterations < 20)
            {
                var neighbours = map.NeighbourNodes(map[newX, newY]);

                // Check which neighbour node that leads furthest away from the target.
                foreach (Node neighbour in neighbours)
                {
                    if (neighbour.Closed) continue;

                    var tempDist = neighbour.DistanceTo(targetNode);

                    if (tempDist > currentDist)
                    {
                        currentDist = tempDist;
                        var optimalNeighbour = neighbours.IndexOf(neighbour);

                        newX = neighbours[optimalNeighbour].X;
                        newY = neighbours[optimalNeighbour].Y;
                    }
                }

                iterations++;
            }

            // Find the path to the target location.
            Path = AStar.FindPathTo(map[X, Y], map[newX, newY], map);
        }
    }
}
