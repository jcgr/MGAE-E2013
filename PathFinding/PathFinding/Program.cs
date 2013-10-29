using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathFinding
{
    class Program
    {
        static void Main(string[] args)
        {
            var map = new Map(10, 15);
            var pathFinder = new AStar();

            Console.WriteLine("Partfinding starting");
            var path = pathFinder.FindPath(map[2, 2], map[8, 13], map);
            Console.WriteLine("Pathfinding done");

            foreach (var node in path)
            {
                Console.WriteLine(node.X + " - " + node.Y);
            }
            Console.ReadLine();
        }
    }
}
