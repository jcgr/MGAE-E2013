namespace PathFinding
{
    class Agent
    {
        public int X { get; set; }
        public int Y { get; set; }

        public int Speed { get; set; }

        public Agent()
            : this (0, 0, 1)
        {
        }

        public Agent(int x, int y, int speed)
        {
            X = x;
            Y = y;
            Speed = speed;
        }
    }
}
