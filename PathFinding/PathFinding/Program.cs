using System;
using System.Collections.Generic;
using System.Threading;

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
            LoadAgents();

            // Draw
            _map.Draw(_agentA, _agentB, new List<Node>(), new List<Node>());
            Console.ReadLine();

            // Run until A catches B.
            while (true)
            {
                // Make B avoid A
                _agentB.Avoid(_agentA, _map);
                var agentBPath = _agentB.Path;

                var speed = (_agentB.Speed) <= agentBPath.Count - 1 ? _agentB.Speed : agentBPath.Count - 1;
                var newPos = agentBPath[speed];
                _agentB.X = newPos.X;
                _agentB.Y = newPos.Y;
                agentBPath.RemoveRange(0, speed);

                // Make A chase B
                _agentA.Chase(_agentB, _map);
                var agentAPath = _agentA.Path;

                speed = (_agentA.Speed) <= agentAPath.Count - 1 ? _agentA.Speed : agentAPath.Count - 1;
                newPos = agentAPath[speed];
                _agentA.X = newPos.X;
                _agentA.Y = newPos.Y;
                agentAPath.RemoveRange(0, speed);

                // Draw the map
                _map.Draw(_agentA, _agentB, agentAPath, agentBPath);

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
        private static void LoadAgents()
        {
            var defaultASpeed = 1;
            var defaultBSpeed = 1;
            int newASpeed, newBSpeed;

            Console.WriteLine("Enter the speed for agent A (default is 1):");
            var result = Console.ReadLine();
            bool isNumeric = int.TryParse(result, out newASpeed);
            if (isNumeric) defaultASpeed = newASpeed;

            Console.WriteLine("Enter the speed for agent B (default is 1):");
            result = Console.ReadLine();
            isNumeric = int.TryParse(result, out newBSpeed);
            if (isNumeric) defaultBSpeed = newBSpeed;

            _agentA = new Agent(2, 2, defaultASpeed);
            _agentB = new Agent(5, 4, defaultBSpeed);
        }

        /// <summary>
        /// Loads the map.
        /// </summary>
        private static void LoadMap()
        {
            _map = new Map(10, 15);

            _map.Close(1, 3);
            _map.Close(2, 3);
            _map.Close(3, 3);
            _map.Close(4, 3);
            _map.Close(5, 3);
            _map.Close(6, 3);
            _map.Close(7, 3);
        }
    }
}
