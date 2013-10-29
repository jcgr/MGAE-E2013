using System;
using System.Collections.Generic;

namespace PathFinding
{
    class Program
    {
        private static readonly Map Map = new Map(10, 15);
        private static readonly AStar PathFinder = new AStar();
        private static Agent _agentA;
        private static Agent _agentB;

        static void Main(string[] args)
        {
            _agentA = new Agent(2, 2, 2);
            _agentB = new Agent(8, 13, 1);

            // Make A chase B.
            while (_agentA.X != _agentB.X
                   || _agentA.Y != _agentB.Y)
            {
                //Console.WriteLine("Agent A is at: " + _agentA.X + " - " + _agentA.Y + " and moves towards B");
                //Console.WriteLine("Agent B is at: " + _agentB.X + " - " + _agentB.Y + " and runs away from A");



                var agentANode = new Node(_agentA.X, _agentA.Y);
                var agentBNode = new Node(_agentB.X, _agentB.Y);

                var agentAPath = MoveTowards(agentANode, agentBNode);
                var agentBPath = MoveAwayFrom(agentBNode, agentANode);

                Map.Draw(agentANode, agentBNode, agentAPath, agentBPath);

                var speed = (_agentA.Speed) <= agentAPath.Count - 1 ? _agentA.Speed : agentAPath.Count - 1;
                var newPos = agentAPath[speed];

                _agentA.X = newPos.X;
                _agentA.Y = newPos.Y;

                speed = (_agentB.Speed) <= agentBPath.Count - 1 ? _agentB.Speed : agentBPath.Count - 1;
                newPos = agentBPath[speed];

                _agentB.X = newPos.X;
                _agentB.Y = newPos.Y;

                Console.ReadLine();
            }

            Console.WriteLine("Agent A has cought agent B!");
            Console.WriteLine("Agent A: " + _agentA.X + " - " + _agentA.Y);
            Console.WriteLine("Agent B: " + _agentB.X + " - " + _agentB.Y);
            Console.ReadLine();
        }

        private static List<Node> MoveTowards(Node a, Node b)
        {
            return PathFinder.FindPath(Map[a.X, a.Y], Map[b.X, b.Y], Map);
        }

        private static List<Node> MoveAwayFrom(Node b, Node a)
        {
            var list = new List<Node>();

            var newX = b.X;
            var newY = b.Y;

            if (a.X < b.X && Map.WithinMap(b.X + 1, b.Y)) newX++;
            else if (a.X <= b.X && Map.WithinMap(b.X - 1, b.Y)) newX--;
            if (a.Y < b.Y && Map.WithinMap(b.X, b.Y + 1)) newY++;
            else if (a.Y <= b.Y && Map.WithinMap(b.X, b.Y - 1)) newY--;

            list.Add(Map[newX, newY]);
            return list;
        }
    }
}
