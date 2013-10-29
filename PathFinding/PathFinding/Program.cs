using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathFinding
{
    class Program
    {
        private static Map _map = new Map(10, 15);
        private static AStar _pathFinder = new AStar();
        private static Agent _agentA;
        private static Agent _agentB;
        private static Random _random = new Random();

        static void Main(string[] args)
        {
            _agentA = new Agent(2, 2, 2);
            _agentB = new Agent(8, 13, 1);

            while (_agentA.X != _agentB.X
                   || _agentA.Y != _agentB.Y)
            {
                Console.WriteLine("Agent A is at: " + _agentA.X + " - " + _agentA.Y + " and moves towards B");
                Console.WriteLine("Agent B is at: " + _agentB.X + " - " + _agentB.Y + " and runs away from A");

                var agentANode = new Node(_agentA.X, _agentA.Y);
                var agentBNode = new Node(_agentB.X, _agentB.Y);

                MoveTowards(agentANode, agentBNode);
                MoveAwayFrom(agentBNode, agentANode);

                Console.WriteLine("---------------------------");
            }

            Console.WriteLine("Agent A has cought agent B!");
            Console.WriteLine("Agent A: " + _agentA.X + " - " + _agentA.Y);
            Console.WriteLine("Agent B: " + _agentB.X + " - " + _agentB.Y);
            Console.ReadLine();
        }

        private static void MoveTowards(Node a, Node b)
        {
            var path = _pathFinder.FindPath(_map[a.X, a.Y], _map[b.X, b.Y], _map);
            var speed = (_agentA.Speed) <= path.Count - 1 ? _agentA.Speed : path.Count - 1;
            var newPos = path[speed];

            _agentA.X = newPos.X;
            _agentA.Y = newPos.Y;
        }

        private static void MoveAwayFrom(Node b, Node a)
        {
            var newX = b.X;
            var newY = b.Y;

            if (a.X < b.X && _map.WithinMap(b.X + 1, b.Y)) newX++;
            else if (a.X <= b.X && _map.WithinMap(b.X - 1, b.Y)) newX--;
            if (a.Y < b.Y && _map.WithinMap(b.X, b.Y + 1)) newY++;
            else if (a.Y <= b.Y && _map.WithinMap(b.X, b.Y - 1)) newY--;

            var tempNode = _map[newX, newY];

            _agentB.X = tempNode.X;
            _agentB.Y = tempNode.Y;
        }
    }
}
