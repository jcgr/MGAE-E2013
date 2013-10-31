using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Program
    {
        private static Map _map;
        private static Agent _agentA;
        private static Agent _agentB;

        static void Main(string[] args)
        {
            LoadMap();
            Console.WriteLine("If you want to use default values for the agents, press <enter> to skip each value:");
            _agentA = LoadAgent("A", 2, 2, 1);
            _agentB = LoadAgent("B", 5, 7, 1);

            // Draw
            _map.Draw(_agentA, _agentB);
            Console.ReadLine();

            // Run until A catches B.
            while (true)
            {
                // Make B avoid A
                _agentB.Avoid(_agentA, _map);

                // Make A chase B
                _agentA.Chase(_agentB, _map);

                // Draw the map
                _map.Draw(_agentA, _agentB);

                // Stop the loop if A has caught B.
                if (_agentA.X == _agentB.X && _agentA.Y == _agentB.Y) break;

                Console.ReadLine();
            }

            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("Agent A has caught agent B at X:" + _agentA.X + " Y:" + _agentA.Y + "!");
            Console.ReadLine();
        }

        /// <summary>
        /// Loads the agents.
        /// </summary>
        private static Agent LoadAgent(String agent, int defaultX, int defaultY, int defaultSpeed)
        {
            int newPosX, newPosY, newSpeed;

            Console.WriteLine("Enter the X position for agent " + agent + " (between 0 and " + (_map.Height - 1) + "):");
            bool isNumeric = int.TryParse(Console.ReadLine(), out newPosX);
            if (!isNumeric) newPosX = defaultX;

            Console.WriteLine("Enter the Y position for agent " + agent + " (between 0 and " + (_map.Width - 1) + "):");
            isNumeric = int.TryParse(Console.ReadLine(), out newPosY);
            if (!isNumeric) newPosY = defaultY;

            Console.WriteLine("Enter the speed for agent " + agent + " (default is 1):");
            isNumeric = int.TryParse(Console.ReadLine(), out newSpeed);
            if (!isNumeric) newSpeed = defaultSpeed;

            if (_map[newPosX, newPosY].Closed
                || !_map.WithinMap(newPosX, newPosY))
            {
                Console.WriteLine("Agent was placed on a closed/invalid position. Using default position instead.");
                newPosX = defaultX;
                newPosY = defaultY;
            }

            Console.WriteLine();
            return new Agent(newPosX, newPosY, newSpeed);
        }

        /// <summary>
        /// Loads the map.
        /// </summary>
        private static void LoadMap()
        {
            _map = new Map(10, 20);

            _map.Close(1, 5);
            _map.Close(2, 5);
            _map.Close(3, 5);
            _map.Close(4, 5);
            _map.Close(5, 5);
            _map.Close(6, 5);
            _map.Close(7, 5);
        }
    }
}
